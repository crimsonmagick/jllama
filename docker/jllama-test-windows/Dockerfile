FROM mcr.microsoft.com/windows/servercore:ltsc2019

WORKDIR /app

COPY hello.ps1 .

SHELL ["powershell", "-Command", "$ErrorActionPreference = 'Stop'; $ProgressPreference = 'SilentlyContinue';"]

RUN New-Item -ItemType Directory -Path C:\temp -Force
RUN Invoke-WebRequest -Uri "https://download.visualstudio.microsoft.com/download/pr/2d6bb6b2-226a-4baa-bdec-798822606ff1/8494001c276a4b96804cde7829c04d7f/ndp48-x86-x64-allos-enu.exe" -OutFile "C:\\temp\\dotnet-framework-installer.exe"
RUN C:\\temp\\dotnet-framework-installer.exe /q
RUN del C:\\temp\\dotnet-framework-installer.exe

RUN Invoke-Expression ((New-Object System.Net.WebClient).DownloadString('https://chocolatey.org/install.ps1'))

RUN choco feature enable -n=allowGlobalConfirmation ; \
    choco install \
    temurin17 \
    git \
    git-lfs
RUN choco install python --version=3.11.0

RUN Invoke-WebRequest -Uri "https://github.com/ggerganov/llama.cpp/releases/download/b1614/llama-b1614-bin-win-avx2-x64.zip" -OutFile "C:\app\llama.cpp.zip"

RUN Expand-Archive -LiteralPath "llama.cpp.zip" -DestinationPath "llama.cpp"

ARG LLAMA_PATH="C:\\app\\llama.cpp"
RUN "[Environment]::SetEnvironmentVariable('LLAMA_PATH', ${LLAMA_PATH}, [EnvironmentVariableTarget]::Machine)"

RUN git clone -b b1614 https://github.com/ggerganov/llama.cpp llama.cpp.src
ARG LLAMA_SRC="C:\\app\\llama.cpp.src"
RUN "[Environment]::SetEnvironmentVariable('LLAMA_SRC', ${LLAMA_SRC}, [EnvironmentVariableTarget]::Machine)"

RUN Import-Module $env:ChocolateyInstall\helpers\chocolateyProfile.psm1 ; \
    refreshEnv

RUN mkdir models ; \
    git-lfs clone https://huggingface.co/VMware/open-llama-7b-v2-open-instruct models/llama2-instruct

RUN pip install -r $env:LLAMA_SRC\requirements.txt
RUN py $env:LLAMA_SRC\convert.py models\llama2-instruct
RUN . $env:LLAMA_PATH\quantize models\llama2-instruct\ggml-model-f16.gguf models\llama2-instruct-q4_0.gguf q4_0
RUN Remove-Item -Path models\llama2-instruct -Recurse -Force

CMD ["powershell", "-File", "./hello.ps1"]