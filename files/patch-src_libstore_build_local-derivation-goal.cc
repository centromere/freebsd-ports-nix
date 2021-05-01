--- src/libstore/build/local-derivation-goal.cc.orig	2021-04-29 18:26:51 UTC
+++ src/libstore/build/local-derivation-goal.cc
@@ -33,12 +33,16 @@
 #endif
 
 /* Includes required for chroot support. */
-#if __linux__
+#if __linux__ || __FreeBSD__
 #include <sys/socket.h>
 #include <sys/ioctl.h>
 #include <net/if.h>
+#endif
+#if __linux__
 #include <netinet/ip.h>
 #include <sys/personality.h>
+#endif
+#if __linux__ || __FreeBSD__
 #include <sys/mman.h>
 #include <sched.h>
 #include <sys/param.h>
@@ -161,7 +165,7 @@ void LocalDerivationGoal::tryLocalBuild() {
     /* If `build-users-group' is not empty, then we have to build as
        one of the members of that group. */
     if (settings.buildUsersGroup != "" && getuid() == 0) {
-#if defined(__linux__) || defined(__APPLE__)
+#if defined(__linux__) || defined(__APPLE__) || defined(__FreeBSD__)
         if (!buildUser) buildUser = std::make_unique<UserLock>();
 
         if (buildUser->findFreeUser()) {
