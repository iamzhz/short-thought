#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>

int to_page(char *filename);

int main(int argc, char *argv[]) {
    DIR *dir;
    struct dirent *ent;

    printf("<!DOCTYPE html>\n"
        "<html lang=\"en\">\n"
        "<head>\n"
        "    <meta charset=\"UTF-8\">\n"
        "    <title>%s</title>\n"
        "    <style>\n"
        "        body {\n"
        "            background-color: #b2adbb;\n"
        "        }\n"
        "        .date {\n"
        "            font-style: italic;\n"
        "        }\n"
        "        .content {\n"
        "            font-size: 20px;\n"
        "            border: 2px solid #6c757d;\n"
        "            border-radius: 8px;\n"
        "            padding: 10px;\n"
        "            background-color: #f8f9fa;\n"
        "            color: #333;\n"
        "            box-shadow: 2px 2px 8px rgba(0, 0, 0, 0.1);\n"
        "        }\n"
        "    </style>\n"
        "</head>\n"
        "<body>\n", argv[1]);
    if ((dir = opendir(argv[1])) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (!strcmp(ent->d_name, ".") ||!strcmp(ent->d_name, "..")) continue;
            char path[100];
            sprintf(path, "%s/%s", argv[1], ent->d_name);
            to_page(path);
        }
        closedir(dir);
    } else {
        perror("Cannot open directory");
        return 1;
    }
printf("<script>\n"
       "    var divs = document.body.getElementsByTagName('div');\n"
       "    var divArray = Array.prototype.slice.call(divs);\n"
       "    divArray.reverse();\n"
       "    divArray.forEach(function(div) {\n"
       "        div.parentNode.removeChild(div);\n"
       "    });\n"
       "    divArray.forEach(function(div) {\n"
       "        document.body.appendChild(div);\n"
       "    });\n"
       "</script></body>\n"
       "</html>\n");

    return 0;
}
int to_page(char *filename) {
    /* prepare for the input file */
    FILE *fp;
    int len; /* length of the filename */
    len = strlen(filename);
    fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Failed to open file");
        return 1;
    }
    /* deal with the filename: xxx/hello.txt -> xxx.hello */
    char *P = filename;
    while (*P) {
        if (*P == '/') {
            *P = '.';
        }
        P++;
    }
    if (!strcmp(&(filename[len-4]), ".txt")) { /* if endwith ".txt" */
        filename[len-4] = '\0'; /* remove ".txt" */
    }
    /* read the content of the file */
    char content[1024];
    fgets(content, 1024, fp);
    fclose(fp);
    /* generate the HTML code */
    printf("    <div><p class='date'>%s</p>\n"
        "    <p class='content'>%s</p></div>\n", filename, content);
    /* return success */
    return 0;
}