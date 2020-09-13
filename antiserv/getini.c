#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "getini.h"


#define MAX_BUFFER_SIZE 60000
#define MAX_VALUE_BUFFER_SIZE 128

static char s_szbuffer[MAX_BUFFER_SIZE];   /*����ļ�����*/
static int i_buffer_len=0;                 /*����ļ���С*/
static int i_buffer_changed=0;             /*��ŸĶ���־*/
static FILE *fp=NULL;

/********************************************************************
  Function:       OpnIniFile       // ��������
  Description:    ��Ҫ�������ļ�   // �������ܡ����ܵȵ�����
  Calls:                           // �����������õĺ����嵥
  Input:      p_File -- �ļ���(���,����·��)
  Output:                          // �����������˵����
  Return:                          // ��������ֵ��˵��
              0 -- ���ļ��ɹ�
             -1 -- ���ļ�����
             -2 -- ��ȡ����������
             -3 -- �ļ�̫��
  Others:                          // ����˵��
**********************************************************************/
int open_ini_file(const char *pFile)
{
    /*
    stat():�����ļ���״̬(����). 
    ��Ҫ��������:�ļ�·������һ���ṹָ��,ָ��״̬��Ϣ�Ĵ�ŵ�λ��. 
    �ṹ��������: 
    st_mode: �ļ����ͺʹ�ȡ��ɷ�ʽ 
    st_ino: I�ڵ�� 
    st_dev: �ļ������豸��ID 
    st_rdev: �ر��ļ���ID 
    st_nlink: �ļ������� 
    st_uid: �ļ������ߵ�UID 
    st_gid: �ļ�С���GID 
    st_size: ���ֽڼ������ļ���С 
    st_atime: ����ȡʱ��(��) 
    st_mtime: ����޸�ʱ��(д)�����״̬�ĸı� 
    st_ctime: ����״̬�޸�ʱ�� 
    ����ֵ: 0:�ɹ� 1:ʧ�� 
    */

    struct stat statbuf; 

    stat(pFile,&statbuf);
    if ( statbuf.st_size > MAX_BUFFER_SIZE )
    {
       fprintf( stderr,"�����ļ�̫������С�����ļ���\n");
       fflush(stderr);
       return -3;
    }

    i_buffer_len = 0; 
    i_buffer_changed = 0; 
    if ( NULL == (fp=fopen(pFile, "rt")) ) 
    {
        printf("�ļ������ڻ���ļ�����!\n");
        return -1; 
    }

    /*����ָ�����ȵ�λ����ļ�β EOF*/
    /*int fread(void *ptr,int size,int nitems,FILE *stream)*/ 
    /*����stream�ж���nitems������Ϊsize���ַ�������ptr��*/
	//i_buffer_len = fread(s_szbuffer,statbuf.st_size,1,fp);
    if ( 0 != fread(s_szbuffer,statbuf.st_size,1,fp) )
    { 
        if (fp != NULL) 
        {
            fclose(fp); 
        }

        fp = NULL; 
        return -2; 
    } 

    i_buffer_len = statbuf.st_size; 
    return 0; 
}

/********************************************************************
  Function:       CloseIniFile     // ��������
  Description:    �ر��ļ�         // �������ܡ����ܵȵ�����
  Calls:                           // �����������õĺ����嵥
  Input:                           // �����������˵��
  Output:                          // �����������˵��
  Return:                          // ��������ֵ��˵��
              0 -- �ر��ļ��ɹ�
             -1 -- �ر��Ѵ򿪵��ļ�����
  Others:                          // ����˵��
**********************************************************************/
int close_ini_file(void) 
{ 
    if ( fp != NULL)
    {   /*���������������ݱ��ı�,��д���ļ��ٹر�*/
        if (i_buffer_changed) 
        { 
            /*����ǰ�ļ�ָ��fp�Ƶ��ļ���ͷ*/
            rewind(fp); 
            /*int fwrite(void *ptr,int size,int nitems,FILE *stream)*/
            /*����stream��д��nitems������Ϊsize���ַ���,�ַ�����ptr��*/
            fwrite(s_szbuffer, i_buffer_len, 1, fp); 
        } 

        if (!fclose(fp)) 
        {
            return 0; 
        }
        else 
        {
            return -1; 
        }

    } 
    else
    {
        return 0; 
    }
} 

/********************************************************************
  Function:       GetIniString     // ��������
  Description:    ��ȡ�ַ�������   // �������ܡ����ܵȵ�����
  Calls:                           // �����������õĺ����嵥
  Input:                           // �����������˵��
    char *s_section : Section Name  // ����
    char *s_key: Identity Name    // �ؼ�����
  Output:                          // �����������˵��
    char *s_result: Returned string // ����ֵ
  Return:                          // ��������ֵ��˵��
              0 -- ��ȡ��Ӧֵ�ɹ�
             -1 -- ��ȡ��Ӧֵʧ��
  Others:                          // ����˵��
**********************************************************************/
int get_ini_string(const char *s_section,const char *s_key, char *s_result)
{
    int i=0;                    /*���ڿ��ƶ�ȡ�ַ�*/
    int j=0;                    /*�����ҵ�ֵ���ָ�븳ֵ*/
    int min;                    /*���ڴ�Ŷ�ȡֵ��λ��*/
    int i_key_found=-1;         /*�Ƿ��ҵ������ı�־*/
    
    /*���������ؼ��ֻ��ļ�Ϊ��,����-1*/
    if (!strlen(s_section) || !strlen(s_key) || (fp == NULL))
    {
        return -1; 
    }

    while ( i < i_buffer_len ) 
    { 
        /*��ȡ��ͷΪ' '��'\t')�ж���λ,ָ������ƶ�iλ*/
        while ((i<i_buffer_len) && ((s_szbuffer[i]==' ') || (s_szbuffer[i]=='\t')))
        {
            i++; 
        }

        /*���i�������ļ��ܳ���,�򷵻�-1*/
        if (i>=i_buffer_len) 
        {
            return -1; 
        }

        if ( '#' == s_szbuffer[i]) 
        {    /*������'#'��ͷ�����,ֱ��\n*/
            while ((i<i_buffer_len) && (s_szbuffer[i] != '\n')) 
            {
                i++; 
            }

            if (i>=i_buffer_len) 
            {
                return -1; 
            }

            /*�����\n,��������һ��*/
            i++; 
        } 
        else 
        { 
            if ('[' == s_szbuffer[i]) 
            { 
                i++; 
                /*���[]��ǰ�����''��\t��������*/
                while ((i<i_buffer_len) &&((s_szbuffer[i]==' ') || (s_szbuffer[i]=='\t'))) 
                {
                    i++; 
                }

                if (i>=i_buffer_len) 
                {
                    return -1; 
                }

                if (0 == strncmp(s_szbuffer+i, s_section, strlen(s_section))) 
                { 
                    /*���ļ����ݵ�iλ��ʼ������Ķ����Ƚ�,�����ͬ*/
                    i += strlen(s_section); 
                    /*���[]�к������''��\t��������*/
                    while ((i<i_buffer_len) && ((s_szbuffer[i]==' ') || (s_szbuffer[i]=='\t'))) 
                    {
                        i++; 
                    }

                    if (i>=i_buffer_len)
                    {
                        return -1; 
                    }

                    /*�����ȡ����']',��ı�i_key_found=0*/
                    if (']' == s_szbuffer[i]) 
                    {    
                        i_key_found=0; 
                    }

                    i++;
                    
                    /*�ƶ�ָ��ֱ����ȡ��'\n'*/
                    while ((i<i_buffer_len) && (s_szbuffer[i]!='\n'))
                    {
                        i++; 
                    }

                    if (i>=i_buffer_len) 
                    {
                        return -1; 
                    }
                    
                    /*����һ��*/
                    i++; 
                } 
                else 
                {   /*���û���ҵ��Ͷ���ƥ��,��һֱ�ƶ�ָ�뵽'\n'*/
                    while ((i<i_buffer_len) && (s_szbuffer[i]!='\n')) 
                    {
                        i++; 
                    }

                    if (i>=i_buffer_len) 
                    {
                        return -1; 
                    }

                    /*����һ��*/
                    i++; 
                } 
            } 
            else 
            { 
                if (0 != i_key_found) 
                {   
                    /*���i_key_foundΪ0��˵��û���ҵ�ƥ��Ķ���,�������Ƶ�'\n'*/
                    while ((i<i_buffer_len) && (s_szbuffer[i] != '\n')) 
                    {
                        i++; 
                    }

                    if (i>=i_buffer_len) 
                    {    
                        return -1; 
                    }

                    /*����һ��*/
                    i++; 
                } 
                else 
                {   /*������˶���ƥ��,��ȽϹؼ���*/
                    if (0 == strncmp(s_szbuffer+i, s_key, strlen(s_key)))
                    { 
                        i += strlen(s_key); 
                        if (i>=i_buffer_len)
                        {
                            return -1; 
                        }
                        while ((i<i_buffer_len) && ((s_szbuffer[i]=='\t') || (s_szbuffer[i]==' '))) 
                        {
                            i++; 
                        }

                        if ( '=' == s_szbuffer[i]) 
                        {   /*��ȡ=��˵���ҵ��˹ؼ�����*/
                            i++; 

                            /*��Ҫ��ȡ��ֵǰ���''��'\t'�����˵�*/
                            while ((i<i_buffer_len) && 
                                   ((s_szbuffer[i]=='\t') || 
                                   (s_szbuffer[i]==' '))) 
                            {
                                i++; 
                            }

                            if (i>=i_buffer_len) 
                            {
                                return -1; 
                            }

                            j=i; 
                            /*��һָ�벻Ϊ'\n'���Ҳ�Ϊ'#',�����¶�ȡ*/
                            while ((j<i_buffer_len) && 
                                   (s_szbuffer[j]!='\n') && 
                                   (s_szbuffer[j]!='#'))
                            {
                                j++; 
                            }

                            j--; 
                            while ((s_szbuffer[j]==' ') || 
                                   (s_szbuffer[j]=='\t')) 
                            {
                                 j--; 
                            }
                            /*�����ȡ����ֵ���ܳ���*/
                            min = j-i+1; 
                            /*�����copy��s_result��,����ĩβ��'\0',����0,�ɹ�*/
                            strncpy(s_result, s_szbuffer+i, min); 
                            *(s_result+min) = '\0'; 
                            return 0; 
                        } 
                        else 
                        {   /*���û���ҵ�=��������ָ��*/
                            while ((i<i_buffer_len) && (s_szbuffer[i]!='\n')) 
                            {
                               i++; 
                            }

                            if (i>=i_buffer_len) 
                            {
                               return -1; 
                            }

                            /*����һ��*/
                            i++; 
                        }    /*û���ҵ�'='��*/ 
                    }
                    else 
                    {    /*û���ҵ��ؼ���,������ָ��*/
                        while ((i<i_buffer_len) && (s_szbuffer[i]!='\n'))
                        {
                            i++; 
                        }
                        
                        if (i>=i_buffer_len) 
                        {
                            return -1; 
                        }

                        /*�Ƶ���һ��*/
                        i++; 
                    } 
                } 
            } 
        } 
    } /* while (i<i_buffer_len)  */
    /*���û��return0��˵���ҵ�,����-1*/
    return -1; 
} 

/********************************************************************
  Function:       CFG_get_key      // ��������
  Description:    ��ȡ�ַ�������   // �������ܡ����ܵȵ�����
  Calls:                           // �����������õĺ����嵥
  Input:                           // �����������˵��
    char *pFile: File Name         // �ļ�·����
    char *section : Section Name   // ����
    char *key: Identity Name       // �ؼ�����
  Output:                          // �����������˵��
    char *val: Returned string     // ����ֵ
  Return:                          // ��������ֵ��˵��
              0 -- ��ȡ��Ӧֵ�ɹ�
             -1 -- ��ȡ��Ӧֵʧ��
  Others:                          // ����˵��
**********************************************************************/
int cfg_get_key(const char *pFile, const char *s_section, 
                const char *s_key, char *s_val)
{
    int ret_code;
    ret_code = open_ini_file(pFile);

    if ( 0 == ret_code)
    {
        ret_code = get_ini_string(s_section,s_key,s_val);
        if (0 == ret_code)
        {
            /*�ҵ�ֵ��*/
            close_ini_file();
            return 0;
        }
        else
        {
            close_ini_file();
            printf("��%s�е�[%s]��û���ҵ�%s��ֵ.\n",pFile,s_section,s_key);
            return -1;
        }
    }
    close_ini_file();
    return -1;
}

/********************************************************************
  Function:       Get_get_ini      // ��������
  Description:    ��ȡ��������     // �������ܡ����ܵȵ�����
  Calls:                           // �����������õĺ����嵥
  Input:                           // �����������˵��
    char *pFile: File Name         // �ļ�·����
    char *section : Section Name   // ����
    char *key: Identity Name       // �ؼ�����
    int iDefVal                    // Ĭ�Ϸ�������ֵ
  Output:                          // �����������˵��
  Return:                          // ��������ֵ��˵��
    s_temp                         // ���ػ�ȡֵ
    iDefVal                        // ����Ĭ��ֵ
  Others:                          // ����˵��
**********************************************************************/
int cfg_get_int(const char *pFile, const char *s_section, const char *s_key)
{
    char s_temp[MAX_VALUE_BUFFER_SIZE];

    if (0 ==  cfg_get_key(pFile,s_section,s_key,s_temp))
    {
        /*�����ȡ����'0x'��'0X'��Ϊ16������,ת����ʮ������*/
        if (strlen(s_temp)>2)
        {
            if ((s_temp[0]=='0') && ((s_temp[1]=='x') || (s_temp[1]=='X')))
            {
                /*���ַ���ͨ��16����ת��������*/
                return (int)(strtol(s_temp,(char **)NULL,16));
            }
        }
        return atoi(s_temp);
    }
    return 0;
}

/********************************************************************
  Function:       Get_get_long     // ��������
  Description:    ��ȡ����������   // �������ܡ����ܵȵ�����
  Calls:                           // �����������õĺ����嵥
  Input:                           // �����������˵��
    pFile     -- �ļ�·����
    section   -- ����
    key       -- �ؼ�����
  Output:                          // �����������˵��
  Return:                          // ��������ֵ��˵��
                                // ���ػ�ȡֵ
  Others:                          // ����˵��
**********************************************************************/
long cfg_get_long(const char *pFile, const char *s_section, const char *s_key)
{
    char s_temp[MAX_VALUE_BUFFER_SIZE];

    if (0 == cfg_get_key(pFile,s_section,s_key,s_temp))
    {
        /*�����ȡ����'0x'��'0X'��Ϊ16������,ת����ʮ������*/
        if (strlen(s_temp)>2)
        {
            if ((s_temp[0]=='0') && ((s_temp[1]=='x') || (s_temp[1]=='X')))
            {
                /*���ַ���ͨ��16����ת���ɳ�����*/
                return (long)(strtol(s_temp,(char **)NULL,16));
            }
        }
        return atol(s_temp);
    }
    return 0;
}

/********************************************************************
  Function:       Get_get_double   // ��������
  Description:    ��ȡʵ������     // �������ܡ����ܵȵ�����
  Calls:                           // �����������õĺ����嵥
  Input:                           // �����������˵��
    char *pFile: File Name         // �ļ�·����
    char *section : Section Name   // ����
    char *key: Identity Name       // �ؼ�����
  Output:                          // �����������˵��
  Return:                          // ��������ֵ��˵��
    s_temp                         // ���ػ�ȡֵ
  Others:                          // ����˵��
**********************************************************************/
double cfg_get_double(const char *pFile,const char *s_section,const char *s_key)
{
    char s_temp[MAX_VALUE_BUFFER_SIZE];

    if (0 ==  cfg_get_key(pFile,s_section,s_key,s_temp))
    {
        return atof(s_temp);   /*����ȡ���ַ�����atofװ����double��*/
    }

    return 0.0;
}
