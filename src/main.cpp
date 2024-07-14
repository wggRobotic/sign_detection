#include <cstdio>
#include <cstdlib>
#include <dirent.h>
#include <opencv2/highgui.hpp>
#include <stdio.h>

// ABSOLUTE PATH REQUIRED
#define IMAGE_DIR "/bigssd/Dokumente/git/sign_detection/signs"

int main(int argc, char **argv) {
  // Find all files in dir
  DIR *dir;
  char **filenames;
  int filenames_len = 0;
  filenames = (char **)malloc(sizeof(char *));
  dir = opendir(IMAGE_DIR);
  {
    struct dirent *current_file;
    int i = 0;
    while ((current_file = readdir(dir)) != NULL) {
      filenames = (char **)realloc(filenames, (i + 1) * sizeof(char *));

      char *filename_stringed;
      filename_stringed = (char *)malloc(strlen(current_file->d_name) + 1);
      strcpy(filename_stringed, current_file->d_name);

      filenames[i] = filename_stringed;
      i++;
      filenames_len++;
    }
  }

  // Check image for signs
  printf("Filenames_len: %d\n", filenames_len);
  for (int i = 0; i < filenames_len; i++) {
    // cv::Mat image;
    // image = cv::imread(filenames[i]);
  }

  return 0;
}
