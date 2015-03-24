#include <stdio.h>
#include <iconv.h>
#include <stdlib.h>
#include <langinfo.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include "tdebug.h"
#include "tstring.h"
#include "localehelper.h"
#define OUTLEN 255

namespace LocaleHelper
{

using namespace TagLib;

/*!
 ���ַ����������Ա���ת��
 param from  ԭʼ���룬����"GB2312",�İ���iconv֧�ֵ�д
 param to      ת����Ŀ�ı���
 param save  ת��������ݱ��浽���ָ�����Ҫ���ⲿ�����ڴ�
 param savelen �洢ת�������ݵ��ڴ��С
 param src      ԭʼ��Ҫת�����ַ���
 param srclen    ԭʼ�ַ�������
 */
int
convert(const char *from, const char *to, char *src, int srclen, char* save, int savelen)
{
    iconv_t cd;
    char   *inbuf = src;
    char *outbuf = save;
    size_t outbufsize = savelen;
    int status = 0;
    size_t  savesize = 0;
    size_t inbufsize = srclen;
    char* inptr = inbuf;
    size_t      insize = inbufsize;
    char* outptr = outbuf;
    size_t outsize = outbufsize;
    
    cd = iconv_open(to, from);
    iconv(cd,NULL,NULL,NULL,NULL);
    if (inbufsize == 0) {
        status = -1;
        goto done;
    }
    while (insize > 0) {
        size_t res = iconv(cd,&inptr,&insize,&outptr,&outsize);
        if (outptr != outbuf) {
            int saved_errno = errno;
            int outsize = outptr - outbuf;
            strncpy(save+savesize, outbuf, outsize);
            errno = saved_errno;
        }
        if (res == (size_t)(-1)) {
            if (errno == EILSEQ) {
                int one = 1;
                iconvctl(cd,ICONV_SET_DISCARD_ILSEQ,&one);
                status = -3;
            } else if (errno == EINVAL) {
                if (inbufsize == 0) {
                    status = -4;
                    goto done;
                } else {
                    break;
                }
            } else if (errno == E2BIG) {
                status = -5;
                goto done;
            } else {
                status = -6;
                goto done;
            }
        }
    }
    status = strlen(save);
done:
    iconv_close(cd);
    //printf("after:outbuf=%s\n",save);
	//printf("after:savelen=%lu\n",strlen(save));
    return status;

}


/* UNICODE��תΪGB2312�� */
int u2g( char *inbuf, int inlen, char *outbuf, int outlen )
{
	return (convert( "utf-8", "gbk", inbuf, inlen, outbuf, outlen ) );
}


/* GB2312��תΪUNICODE�� */
int g2u( char *inbuf, size_t inlen, char *outbuf, size_t outlen )
{
	return (convert( "gbk", "utf-8", inbuf, inlen, outbuf, outlen ) );
}

bool GBK2UTF8(const TagLib::ByteVector &bv, TagLib::ByteVector *out)
{
    return false;
	int rc = 0;
	char outBuff[OUTLEN+1]={0};
	//printf("(char *)bv.data()=%s\n",(char *)bv.data());
	//printf("bv.size()=%u\n",bv.size());

	rc = g2u((char *)bv.data(), bv.size(), outBuff, OUTLEN);
	if(rc < 0)
	{
		//printf("error conver, rc=%d\n", rc);
		return false;
	}
	*out = ByteVector(outBuff, OUTLEN);
    return true;
}

}

