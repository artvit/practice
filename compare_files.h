#ifndef PRACTIE_COMPARE_FILES_H
#define PRACTIE_COMPARE_FILES_H

struct compare_result {
    int pid;
    bool result;
    int bytesInFile1;
    int bytesInFile2;
    char file1[256];
    char file2[256];
};

bool CompareFiles(compare_result& cr);

#endif
