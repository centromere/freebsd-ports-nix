--- src/libfetchers/git.cc.orig	2021-04-30 22:34:22 UTC
+++ src/libfetchers/git.cc
@@ -7,6 +7,10 @@
 
 #include <sys/time.h>
 
+#if __FreeBSD__
+#include <sys/wait.h>
+#endif
+
 using namespace std::string_literals;
 
 namespace nix::fetchers {
