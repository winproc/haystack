#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "format.h"

// commands: add apple
// remove apple
// to 0

int check_cmd(const char* str_a, const char* str_b); // verify command-subject format
size_t len(const char* str); // get length of a null terminated string

size_t len(const char* str) {
    size_t lcnt = 0;
    while (str[lcnt] != '\0') {
        lcnt++;
    }
    return lcnt;
}

int check_cmd(const char* str_a, const char* str_b_cmd) {
    int smb = 1;

    size_t cmdlen = len(str_b_cmd);
    size_t sublen = len(str_a);

    if (sublen < cmdlen) {
        return 0;
    }

    for (int i = 0; i < cmdlen; i++) {
        if (str_b_cmd[i] != str_a[i]) {
            smb = 0;
        }
    }
    
    return smb;
}


int main() {

    const char prefixcmd_add[] = "add";
    const char prefixcmd_remove[] = "remove";
    const char prefixcmd_to[] = "to";
    const char prefixcmd_fw[] = "fw";

    const char prefixcmd_view[] = "view";

    const char prefixcmd_export[] = "export"; // only csv support

    const char prefixcmd_ploc[] = "ploc";
    const char prefixcmd_paddr[] = "paddr"; // unimplemented, todo, get virtual mem addr of pointer

    char cmdstr[1024]; // command buffer, 1kb 
    memset(cmdstr, 0x0, 1024);

    char* liblist[1024]; // pointers to heap-allocated char blocks, 1kb
    memset(liblist, 0x0, 1024);

    int ls_track = 0; // pointer to track lib
    int ls_len = 0; // total length of liblist

    while (cmdstr[0] != 'q') {
        fgets(cmdstr, 1024, stdin);
        if (check_cmd(cmdstr, prefixcmd_add)) {
            size_t sublen = len(cmdstr);
            size_t cmdlen = len(prefixcmd_add);

            if ((sublen - 1) > cmdlen && cmdstr[cmdlen] == ' ') {
                
                char* subject_buffer = (char*)malloc(512);
                for (int i = 1; i < sublen; i++) {
                    subject_buffer[i - 1] = cmdstr[cmdlen + i];
                }

                liblist[ls_track] = subject_buffer;
                printf("Index %d, added subject %s", ls_track, subject_buffer);

                ls_track++;
                ls_len++;
            }
        } else if (check_cmd(cmdstr, prefixcmd_to)) {
            size_t sublen = len(cmdstr);
            size_t cmdlen = len(prefixcmd_to);

            if ((sublen - 1) > cmdlen && cmdstr[cmdlen] == ' ') {
                
                char subject_buffer[16];
                for (int i = 1; i < sublen; i++) {
                    subject_buffer[i - 1] = cmdstr[cmdlen + i];
                }

                ls_track = subject_buffer[0] - '0';
                printf("Set pointer to index %d\n", ls_track);

            }
        } else if (check_cmd(cmdstr, prefixcmd_view)) {
            if (liblist[ls_track] != 0x0) {
                printf("%s", liblist[ls_track]);
            } else {
                printf("Index is empty\n");
            }
        } else if (check_cmd(cmdstr, prefixcmd_remove)) {
            if (liblist[ls_track] != 0x0) {
                free(liblist[ls_track]);
                liblist[ls_track] = 0x0;
                printf("Removed element\n");
            } else {
                printf("Index is free\n");
            }
            
        } else if (check_cmd(cmdstr, prefixcmd_export)) {
            size_t sublen = len(cmdstr);
            size_t cmdlen = len(prefixcmd_export);

            if ((sublen - 1) > cmdlen && cmdstr[cmdlen] == ' ') {
                
                char subject_buffer[16];
                for (int i = 1; i < sublen; i++) {
                    subject_buffer[i - 1] = cmdstr[cmdlen + i];
                }

                if (strcmp(subject_buffer, "csv\n") == 0) {
                    export_csv(liblist, fmin(ls_track, ls_len));
                    printf("Exported to csv file\n");
                } else if (strcmp(subject_buffer, "json\n") == 0) {
                    export_json(liblist, fmin(ls_track, ls_len));
                    printf("Exported to json file\n");
                }

            }
        } else if (check_cmd(cmdstr, prefixcmd_fw)) {
            ls_track++;
            printf("Incremented pointer, index %d\n", ls_track);

        } else if (check_cmd(cmdstr, prefixcmd_ploc)) {
            printf("Pointer index: %d\n", ls_track);
        } else if (check_cmd(cmdstr, prefixcmd_paddr)) {
            printf("Pointer value address: 0x%X\n", liblist[ls_track]);
        }
    } 
    

    

    return 0;
}