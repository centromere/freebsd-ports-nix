--- src/libstore/build/derivation-goal.cc.orig	2021-04-30 22:30:29 UTC
+++ src/libstore/build/derivation-goal.cc
@@ -55,6 +55,10 @@
 #include <sys/sysctl.h>
 #endif
 
+#if __FreeBSD__
+#include <sys/wait.h>
+#endif
+
 #include <pwd.h>
 #include <grp.h>
 
