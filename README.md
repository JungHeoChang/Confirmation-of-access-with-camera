# Confirmation of access with camera
라즈베리파이 카메라를 이용한 출입 확인 프로그램입니다.

# 설명
라즈베리파이에서 인체 감지 센서 혹은 초음파를 통해 사람의 출입 여부를 확인합니다.
<img src="https://user-images.githubusercontent.com/37999695/62915235-85f74c00-bdce-11e9-9eca-62ce35f0c06d.jpg" width="40%">
<img src="https://user-images.githubusercontent.com/37999695/62915748-855fb500-bdd0-11e9-8907-8b3a2e425df5.jpg" width="44%">

만약 사람의 출입이 확인되면 카메라를 이용하여 사진을 찍습니다.
<img src="https://user-images.githubusercontent.com/37999695/62915947-467e2f00-bdd1-11e9-809b-8f317f2125a2.jpg" width="50%">

찍은 사진은 우분투 혹은 다른 라즈베리파이 서버로 전송합니다.
<img src="https://user-images.githubusercontent.com/37999695/62916899-2ea8aa00-bdd5-11e9-8ab7-d171cfb68001.jpg" widht="40%">

서버에서는 파일을 받으면 데이터 베이스에 저장을 합니다.
<img src="https://user-images.githubusercontent.com/37999695/62916960-6e6f9180-bdd5-11e9-815a-b9a45345fe68.jpg" widht="40%">

웹페이지를 만들어 컴퓨터 또는 휴대폰으로 사진을 확인할 수 있습니다.
<img src="https://user-images.githubusercontent.com/37999695/62917125-01103080-bdd6-11e9-94bf-2d553222e6ad.jpg" width="80%">
<img src="https://user-images.githubusercontent.com/37999695/62917146-17b68780-bdd6-11e9-88df-44d274433049.jpg" width="80%">

# 개발 환경
main.cpp 라즈베리파이 라즈비안 C언어로 작성         
server.cpp 우분투에서 C언어로 작성
