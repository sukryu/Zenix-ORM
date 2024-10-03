#!/bin/bash

# 스크립트 실행 중 오류 발생 시 중단
set -e

# 필요한 패키지 업데이트 및 설치
sudo apt update && sudo apt upgrade -y
sudo apt install -y build-essential python3 curl
sudo apt install git

# Node.js LTS 버전 설치 (이미 설치되어 있다면 이 부분은 건너뛰세요)
curl -fsSL https://deb.nodesource.com/setup_lts.x | sudo -E bash -
sudo apt install -y nodejs

# nvm을 사용하고 있다면 다음과 같이 Node.js LTS 버전 설치
# nvm install --lts
# nvm use --lts

git clone https://github.com/sukryu/Zenix-ORM

# 프로젝트 디렉토리 생성 및 이동
cd ~/Zenix-ORM

npm i

# 빌드 전 환경 변수 설정 (필요한 경우)
export CPLUS_INCLUDE_PATH=$CPLUS_INCLUDE_PATH:$(pwd)/node_modules/node-addon-api

# 빌드 클린업 및 구성
npx node-gyp clean
npx node-gyp configure

# 빌드 실행
npx node-gyp build --verbose

# 빌드 완료 메시지 출력
echo "빌드가 성공적으로 완료되었습니다!"