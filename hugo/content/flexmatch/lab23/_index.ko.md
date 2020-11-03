---
title: S3 웹 호스팅
url: /flexmatch/lab23
weight: 40
pre: "<b>2-3. </b>"
---

### S3 웹 호스팅을 통한 랭킹 리더보드 구현 <br/><br/>

앞서 우리는 데이터베이스들을 구축했고, API Gateway 와 Lambda 함수를 통해 유저 정보를 다룰 수 있는 API 를 구축했습니다.    
이번에는 S3 가 호스팅하는 웹 사이트를 생성하고 이를 이용해서 쉽게 랭킹보드를 구현할 것입니다.

1. S3 서비스로 이동하여 웹 사이트에 사용할 S3 버킷을 생성합니다. 버킷 생성 시, public object를 허용하기 위해 Block all public access를 uncheck합니다.

![S3](../../images/flexmatch/lab23/S3-1.png)

2. 실습에 사용되는 파일 중 web 디렉토리가 있을 것입니다. 디렉토리 내의 main.js 파일을 텍스트 편집기로 엽니다.

3. 48번째 줄의 API Endpoint에 위에서 생성한 API Gateway의 Invoke URL로 수정합니다.

![S3](../../images/flexmatch/lab23/S3-2.png)

4. 수정한 main.js를 저장한 뒤 web 디렉토리의 파일을 전부 앞서 생성한 버킷에 업로드해줍니다

5. 업로드할 때 Public read access 권한을 부여합니다.

6. 버킷에서 Static website hosting을 활성화해주어야 합니다. 버킷의 Properties 탭으로 이동합니다.

![S3](../../images/flexmatch/lab23/S3-3.png)

7. Static website hosting을 활성화합니다. Index document에는 index.html을 입력한 뒤 Save 버튼을 클릭합니다.

![S3](../../images/flexmatch/lab23/S3-4.png)

8. 파일 업로드를 완료하면 Static website hosting의 Endpoint로 접속하여 단순한 웹 페이지를 확인할 수 있습니다.

9. API Gateway로 돌아와서 CORS 설정을 해주어야 합니다. 이를 통해 Ranking board 정보를 웹 페이지에서 읽어 올 수 있게 됩니다.

10. API Gateway로 돌아와서 Actions버튼을 클릭하고 Enable CORS 옵션을 선택합니다.

11. CORS 페이지에서 Access-Control-Allow-Origin 에 static website URL로 수정해줍니다.
(URL 뒤의 / 가 영향을 미칠 수 있기 때문에 확실하지 않으면 기본 값인 * 로 진행합니다.)

![S3](../../images/flexmatch/lab23/S3-5.png)

12. Enable CORS 버튼을 클릭하여 진행합니다.

13. 정상적으로 완료되었다면 왼쪽 탭에 OPTIONS 가 추가된 것을 확인할 수 있습니다.

14. 마지막으로 Actions 메뉴의 Deploy API 버튼을 클릭하여 prod 단계에 배포합니다.

배포한 Static Page를 웹브라우저에서 열어주거나 API 의 Invoke URL을 브라우저에서 열어주시면 Section 2에서 테스트로 입력했던 데이터가 표시되거나 JSON(직접 Invoke URL을 열었을 경우) 으로 표기 되는 것을 확인 할 수 있습니다. 

이 섹션에서 구성한 웹페이지는 다음과 같이 랭킹 보드를 위한 목적으로 사용될 예정입니다.

![S3](../../images/flexmatch/lab23/S3-6.png)

이제 게임을 위한 기반 작업들을 마무리했습니다. :)    
본격적으로 게임 서버를 구축할 시간입니다.

---
<p align="center">
© 2020 Amazon Web Services, Inc. 또는 자회사, All rights reserved.
</p>
