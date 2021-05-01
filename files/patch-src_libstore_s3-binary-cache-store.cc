--- src/libstore/s3-binary-cache-store.cc.orig	2021-04-29 18:26:51 UTC
+++ src/libstore/s3-binary-cache-store.cc
@@ -45,7 +45,7 @@ R && checkAws(const FormatOrString & fs, Aws::Utils::O
     if (!outcome.IsSuccess())
         throw S3Error(
             outcome.GetError().GetErrorType(),
-            fs.s + ": " + outcome.GetError().GetMessage());
+            fs.s + ": " + outcome.GetError().GetMessage().c_str());
     return outcome.GetResultWithOwnership();
 }
 
@@ -55,7 +55,7 @@ class AwsLogger : public Aws::Utils::Logging::Formatte
 
     void ProcessFormattedStatement(Aws::String && statement) override
     {
-        debug("AWS: %s", chomp(statement));
+        debug("AWS: %s", chomp((const std::string &)statement));
     }
 
 #if !(AWS_VERSION_MAJOR <= 1 && AWS_VERSION_MINOR <= 7 && AWS_VERSION_PATCH <= 115)
@@ -146,8 +146,8 @@ S3Helper::FileTransferResult S3Helper::getObject(
 
     auto request =
         Aws::S3::Model::GetObjectRequest()
-        .WithBucket(bucketName)
-        .WithKey(key);
+        .WithBucket(bucketName.c_str())
+        .WithKey(key.c_str());
 
     request.SetResponseStreamFactory([&]() {
         return Aws::New<std::stringstream>("STRINGSTREAM");
@@ -162,7 +162,7 @@ S3Helper::FileTransferResult S3Helper::getObject(
         auto result = checkAws(fmt("AWS error fetching '%s'", key),
             client->GetObject(request));
 
-        res.data = decompress(result.GetContentEncoding(),
+        res.data = decompress(result.GetContentEncoding().c_str(),
             dynamic_cast<std::stringstream &>(result.GetBody()).str());
 
     } catch (S3Error & e) {
@@ -265,8 +265,8 @@ struct S3BinaryCacheStoreImpl : virtual S3BinaryCacheS
 
         auto res = s3Helper.client->HeadObject(
             Aws::S3::Model::HeadObjectRequest()
-            .WithBucket(bucketName)
-            .WithKey(path));
+            .WithBucket(bucketName.c_str())
+            .WithKey(path.c_str()));
 
         if (!res.IsSuccess()) {
             auto & error = res.GetError();
@@ -332,7 +332,7 @@ struct S3BinaryCacheStoreImpl : virtual S3BinaryCacheS
 
             std::shared_ptr<TransferHandle> transferHandle =
                 transferManager->UploadFile(
-                    istream, bucketName, path, mimeType,
+                    istream, bucketName.c_str(), path.c_str(), mimeType.c_str(),
                     Aws::Map<Aws::String, Aws::String>(),
                     nullptr /*, contentEncoding */);
 
@@ -350,13 +350,13 @@ struct S3BinaryCacheStoreImpl : virtual S3BinaryCacheS
 
             auto request =
                 Aws::S3::Model::PutObjectRequest()
-                .WithBucket(bucketName)
-                .WithKey(path);
+                .WithBucket(bucketName.c_str())
+                .WithKey(path.c_str());
 
-            request.SetContentType(mimeType);
+            request.SetContentType(mimeType.c_str());
 
             if (contentEncoding != "")
-                request.SetContentEncoding(contentEncoding);
+                request.SetContentEncoding(contentEncoding.c_str());
 
             request.SetBody(istream);
 
@@ -428,9 +428,9 @@ struct S3BinaryCacheStoreImpl : virtual S3BinaryCacheS
             auto res = checkAws(format("AWS error listing bucket '%s'") % bucketName,
                 s3Helper.client->ListObjects(
                     Aws::S3::Model::ListObjectsRequest()
-                    .WithBucket(bucketName)
+                    .WithBucket(bucketName.c_str())
                     .WithDelimiter("/")
-                    .WithMarker(marker)));
+                    .WithMarker(marker.c_str())));
 
             auto & contents = res.GetContents();
 
@@ -439,8 +439,8 @@ struct S3BinaryCacheStoreImpl : virtual S3BinaryCacheS
 
             for (auto object : contents) {
                 auto & key = object.GetKey();
-                if (key.size() != 40 || !hasSuffix(key, ".narinfo")) continue;
-                paths.insert(parseStorePath(storeDir + "/" + key.substr(0, key.size() - 8) + "-" + MissingName));
+                if (key.size() != 40 || !hasSuffix(key.c_str(), ".narinfo")) continue;
+                paths.insert(parseStorePath(storeDir + "/" + key.substr(0, key.size() - 8).c_str() + "-" + MissingName));
             }
 
             marker = res.GetNextMarker();
