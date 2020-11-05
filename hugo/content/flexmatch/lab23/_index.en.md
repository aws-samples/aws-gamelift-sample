---
title: S3 Web Hosting
url: /flexmatch/lab23
weight: 40
pre: "<b>2-3. </b>"
---

### Leaderboard with S3 Web Hosting<br/><br/>

Previously we made database, API to handle user data and game result with API Gateway and Lambda function.
In this page, we are going to make static web site that S3 host. We can make Ranking board easily with this static web page with Lambda function that we made before.

1. Move to S3 service and create S3 bucket for web site. Creating bucket, uncheck *Block all public access* to allow public object.

![S3](../../images/flexmatch/lab23/S3-1[en].png)

2. There is web directory in the given source code. You can find main.js in this folder. Open main.js from Text editor. 

3. Edit API Endpoint in source code. Modify URL to Invoke URL of API Gateway that we made previously.

![S3](../../images/flexmatch/lab23/S3-2.png)

4. Save modified main.js and upload all files in web directory to the S3 bucket that we made.

5. Give Public read access permission to objects.

6. Click Properties tab in bucket. Enable Static website hosting on this bucket.

7. Put index.html on Index document and click "Save" button.

![S3](../../images/flexmatch/lab23/S3-4[en].png)

8. When your files are uploaded, you are able to check web page on Static website hosting endpoint.

9. You should configure CORS on API Gateway. You can check Ranking board data from web page by this endpoint.

10. Move back to API Gateway, click "Actions" button and enable CORS option. (If it is disabled)

11. Configuring CORS. Set Access-Control-Allow-Origin to your static website URL. (In this HoL, it is OK to configure * for your comfort)

![S3](../../images/flexmatch/lab23/S3-5[en].png)

12. Click "Enable CORS" button.

13. If it is completed, you are able to find OPTIONS in your resource tab.

14. Click "Deploy API" button to deploy it prod stage.

You are able to check its data from API endpoint or this web page.

Web page made on this section will be used for ranking leaderboard like below.

![S3](../../images/flexmatch/lab23/S3-6.png)

**Now, it is time to make Game Server! :)**

---
<p align="center">
© 2020 Amazon Web Services, Inc. 또는 자회사, All rights reserved.
</p>
