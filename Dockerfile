# TODO: Can we use nanocore here?
FROM mcr.microsoft.com/windows:ltsc2019

RUN \
    curl -sSL --output vs_buildtools.exe https://aka.ms/vs/16/release/vs_buildtools.exe && \
    start /w vs_buildtools.exe --quiet --wait --norestart --nocache \
        --installPath C:\BuildTools \
        --add Microsoft.VisualStudio.Workload.VCTools \
        --add Microsoft.VisualStudio.Component.VC.Tools.x86.x64 \
        --add Microsoft.Component.VC.Runtime.UCRTSDK \
        --add Microsoft.VisualStudio.Component.Windows10SDK.20348 \
    && del /q vs_buildtools.exe

# ENV is broken on Windows containers and can't read existing environment variables
RUN setx /M PATH "%PATH%;C:\BuildTools\Common7\Tools;C:\BuildTools\VC\Tools\MSVC\14.29.30133\bin\Hostx64\x64"

WORKDIR /src
ADD https://gnu.mirror.constant.com/make/make-4.4.tar.gz .
RUN tar -xzf make-4.4.tar.gz && del /q make-4.4.tar.gz
RUN cd make-4.4 && VsDevCmd.bat && build_w32.bat && copy WinRel\gnumake.exe ..\make.exe

ADD . .
CMD VsDevCmd.bat && make TOOLCHAIN=MSVC
