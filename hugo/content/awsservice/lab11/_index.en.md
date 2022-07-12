---
title: Creating DynamoDB
url: /awsservice/lab11
weight: 20
pre: "<b>1-1. </b>"
---

### Creating DynamoDB <br/><br/>

This lab uses DynamoDB to store User data and Game result. We are going to store simple information, and this information will be used to Leaderboard on our static web page.

Additionally, we will use ElastiCache in front of DynamoDB for caching. Because your game "Gomoku" will be very popular, so we need cache layer for performance and reliability!

First things first, let's make DynamoDB first.

1. Login to AWS Console and move to DynamoDB page (https://console.aws.amazon.com/dynamodb)
2. Check region. Every resources on this lab should be launched on same region.
3. Click "Create Table" to make DynamoDB Table.

![DDB](../../images/awsservice/lab11/DDB_Create[en].png)

4. Set Table name as “GomokuPlayerInfo”, and Primary Key "PlayerName". Make data type as "String". Let's click "Create button".

5. Enable “Stream” on your DynamoDB Table. It is disabled as default, so select "Exports and streams" tab and click "Enable" button on DynamoDB stream details.


![DDB_Stream](../../images/awsservice/lab11/DDB_Stream[en].png)

6. Select "New and old images", and Click "Enable" button on the box.

![DDB_Stream_Image](../../images/awsservice/lab11/DDB_Stream_Image[en].png)

7. Basic configuration is ended. Let's make Test Data Sample. Click "Explore table Items" in the upper right corner.

8. Click "Create Item", and make new Item.

![DDB_Item](../../images/awsservice/lab11/DDB_Item[en].png)

9. Click Create Item button on the editor, and append item.

10. Put data on your new item same as below. Click "Create Item" to store Item. (Check the data type)

![DDB_Item_Sample](../../images/awsservice/lab11/DDB_Item_Sample[en].png)

* You can find this Item on the table, when you select table GomokuPlayerInfo and click run on Scan/Query items search items from this table.

Ok. This is Basic settings of DynamoDB. Nextly we are making ElastiCache.

---
<p align="center">
© 2020 Amazon Web Services, Inc. 또는 자회사, All rights reserved.
</p>
