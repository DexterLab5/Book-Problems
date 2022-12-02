#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define NULL        0
#define EOF         (-1)
#define BUFSIZ      1024
#define OPEN_MAX    20      /* max #files open at once */

enum _flags {
    _READ   = 01,   /* file open for reading */
    _WRITE  = 02,   /* file open for writing */
    _UNBUF  = 03,   /* file is unbuffered */
    _EOF    = 010,  /* EOF has occurred on this file */
    _ERR    = 020   /* error occurred on this file */
};

typedef struct _iobuf {
    int cnt;        /* characters left */
    char *ptr;      /* next character position */
    char *base;     /* location of buffer */
    int flag;       /* mode of file access */
    int fd;         /* file descriptor */
} FILE;
extern FILE _iob[OPEN_MAX];

#define stdin       (&_iob[0])
#define stdout      (&_iob[1])
#define stderr      (&_iob[2])

int _fillbuf(FILE*);
int _flushbuf(int, FILE*);

#define feof(p)     (((p)->flag & _EOF) != 0)
#define ferror(p)   (((p)->flag & _ERR) != 0)
#define fileno(p)   ((p)->fd)

#define getc(p)     (--(p)->cnt >= 0 \
                ? (unsigned char) *(p)->ptr++ : _fillbuf(p))
#define putc(x,p)   (--(p)->cnt >= 0 \
                ? *(p)->ptr++ = (x) : _flushbuf((x),p))

#define getchar()   getc(stdin)
#define putchar(x)  putc((x), stdout)   

#define PERMS 0666  /* RW for owner, group, others */

FILE _iob[OPEN_MAX] = {  /* stdin, stdout, stderr */
    { 0, (char*) 0, (char*) 0, _READ, 0 },
    { 0, (char*) 0, (char*) 0, _WRITE, 1 },
    { 0, (char*) 0, (char*) 0, _WRITE | _UNBUF, 2 }
};

int _seek_origin = 0, _seek_offset = 0;

FILE *fopen(char *name, char *mode);
int fclose(FILE *fp);
int fseek(FILE *fp, long offset, int origin);

int main(void) {
    FILE *fp = fopen("f1.c", "r");
    FILE *fp2 = fopen("f2.c", "w");
    
    fseek(fp, 6, 0);
    char c = getc(fp);
    putc(c, fp2);
    putc('\n', fp2);
    fseek(fp, 6, 3);
    c = getc(fp);
    putc(c, fp2);
    for (int i = 0; i < 50; i++) {
        c = getc(fp);
        putc(c, fp2);
    }
    
    fclose(fp);
}

/* fopen: open file, return file ptr */
FILE *fopen(char *name, char *mode) {
    int fd;
    FILE *fp;

    if (*mode != 'r' && *mode != 'w' && *mode != 'a')
        return NULL;
    for (fp = _iob; fp < _iob + OPEN_MAX; fp++) {
        if ((fp->flag & (_READ | _WRITE)) == 0)
            break;
    }
    if (fp >= _iob + OPEN_MAX)      /* no free slots */
        return NULL;

    if (*mode == 'w')
        fd = creat(name, PERMS);
    else if (*mode == 'a') {
        if ((fd = open(name, O_WRONLY, 0)) == -1)
            fd = creat(name, PERMS);
        lseek(fd, 0L, 2);
    }
    else
        fd = open(name, O_RDONLY, 0);
    if (fd == -1)       /* couldn't access name*/
        return NULL;

    fp->fd = fd;
    fp->cnt = 0;
    fp->base = NULL;
    fp->flag = (*mode == 'r') ? _READ : _WRITE;
    return fp;
}

/* _fillbuf: allocate and fill input buffer*/
int _fillbuf(FILE *fp) {
    int bufsize;

    if ((fp->flag & (_READ | _EOF | _ERR)) != _READ)
        return EOF;
    bufsize = (fp->flag & _UNBUF) ? BUFSIZ : 1;
    if (fp->base == NULL) {         /* no buffer yet */
        if ((fp->base = (char*) malloc(bufsize)) == NULL)
            return EOF;     /* can't get buffer for some reason */
    }

    fp->ptr = fp->base;
    fp->cnt = read(fp->fd, fp->ptr, bufsize);
    if (_seek_origin > 0 || _seek_offset > 0) {
        fp->ptr = &fp->base[_seek_origin + _seek_offset];
    }

    if (--fp->cnt < 0) {
        if (fp->cnt == -1)
            fp->flag |= _EOF;
        else 
            fp->flag |= _ERR;
        fp->cnt = 0;
        return EOF;
    }
    return (unsigned char) *fp->ptr++;
}

int _flushbuf(int c, FILE *fp) {
    int n, bfsize;
    unsigned char uc = c;

    /* if fp's flag is not _WRITE return */
    if ((fp->flag & (_WRITE | _EOF | _ERR)) != _WRITE) {
        return EOF;
    }

    if (fp->base == NULL && (fp->flag & _UNBUF) == 0) {
        /* allocate memory for buffer */
        if ((fp->base = malloc(BUFSIZ)) == NULL)
            fp->flag |= _UNBUF;  /* can't get buffer for some reason */
        else {
            fp->ptr = fp->base;
            fp->cnt = BUFSIZ - 1;
        }
    }

    if (fp->flag & _UNBUF) {     /* stdout is unbuffered in this case */
        fp->ptr = fp->base = NULL;
        fp->cnt = 0;
        if (c == EOF) {
            return EOF;
        }
        n = write(fp->fd, &c, 1);
        bfsize = 1;
    } 
    else {
        bfsize = (int)(fp->ptr - fp->base);
        n = write(fp->fd, fp->base, bfsize);
        fp->ptr = fp->base;
        fp->cnt = BUFSIZ - 1;
    }

    if (n == bfsize)
        return c;
    else {
        fp->flag |= _ERR;
        return EOF;
    }
}

int fclose(FILE *fp) {
    if (fp == NULL) {
        return -1;
    }

    close(fp->fd);
    fp->fd = 0;
    free(fp->base);
    fp->base = NULL;
    fp->ptr = NULL;
    fp->cnt = 0;
    fp->flag = 0;
    return 1;
}

int fseek(FILE *fp, long offset, int origin) {
    if (fp == NULL) {
        return NULL;
    }
    
    _seek_origin = origin;
    _seek_offset = offset;
    if (fp->base == NULL) {
        return 1;
    }
    
    fp->ptr = &fp->base[_seek_origin + _seek_offset];
    return 1;
}