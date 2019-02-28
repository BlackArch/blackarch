$NetBSD$

Fix build with giflib-5.x.

--- src/gif2swf.c.orig	2011-01-02 03:30:29.000000000 +0000
+++ src/gif2swf.c
@@ -65,6 +65,17 @@ enum disposal_method {
     RESTORE_TO_PREVIOUS
 };
 
+/* from util/qprintf.c of giflib 5.0.4 */
+void
+PrintGifError(int ErrorCode)
+{
+    char *Err = GifErrorString(ErrorCode);
+
+    if (Err != NULL)
+	fprintf(stderr, "GIF-LIB error: %s.\n", Err);
+    else
+	fprintf(stderr, "GIF-LIB undefined error %d.\n", ErrorCode);
+}
 
 void SetFrameAction(TAG ** t, const char *src, int ver)
 {
@@ -200,6 +211,7 @@ TAG *MovieAddFrame(SWF * swf, TAG * t, c
     SRECT r;
     MATRIX m;
     int fs;
+    int ErrorCode;
 
     U8 *imagedata, *from, *to;
     GifImageDesc *img;
@@ -230,13 +242,13 @@ TAG *MovieAddFrame(SWF * swf, TAG * t, c
     }
     fclose(fi);
 
-    if ((gft = DGifOpenFileName(sname)) == NULL) {
+    if ((gft = DGifOpenFileName(sname, &ErrorCode)) == NULL) {
         fprintf(stderr, "%s is not a GIF file!\n", sname);
         return t;
     }
 
     if (DGifSlurp(gft) != GIF_OK) {
-        PrintGifError();
+        PrintGifError(ErrorCode);
         return t;
     }
 
@@ -455,7 +467,7 @@ TAG *MovieAddFrame(SWF * swf, TAG * t, c
 
     free(pal);
     free(imagedata);
-    DGifCloseFile(gft);
+    DGifCloseFile(gft, NULL);
 
     return t;
 }
@@ -465,6 +477,7 @@ int CheckInputFile(char *fname, char **r
     FILE *fi;
     char *s = malloc(strlen(fname) + 5);
     GifFileType *gft;
+    int ErrorCode;
 
     if (!s)
         exit(2);
@@ -488,7 +501,7 @@ int CheckInputFile(char *fname, char **r
     }
     fclose(fi);
 
-    if ((gft = DGifOpenFileName(s)) == NULL) {
+    if ((gft = DGifOpenFileName(s, &ErrorCode)) == NULL) {
         fprintf(stderr, "%s is not a GIF file!\n", fname);
         return -1;
     }
@@ -499,7 +512,7 @@ int CheckInputFile(char *fname, char **r
         global.max_image_height = gft->SHeight;
 
     if (DGifSlurp(gft) != GIF_OK) { 
-        PrintGifError();
+        PrintGifError(ErrorCode);
         return -1;
     }
     // After DGifSlurp() call, gft->ImageCount become available
@@ -518,7 +531,7 @@ int CheckInputFile(char *fname, char **r
             fprintf(stderr, "frame: %u, delay: %.3f sec\n", i + 1, getGifDelayTime(gft, i) / 100.0);
     }
 
-    DGifCloseFile(gft);
+    DGifCloseFile(gft, NULL);
 
     return 0;
 }
