/*��c�ļ��д����Ϊ�ο�*/ 
#include"head.h"
#include"hanzi.h"
#include"tuxing.h"
#include"welcome.h"

FILE *hzk_p;
char mat0[32],mat1[72];

void hz(int x,int y,char *s,int color,int type)
{
	int a=x;
	switch(type)
	{
		case 16:
			open_hzk("HZK16");
			 break;
		case 24:
			open_hzk("HZK24");
			 break;
	}
	while (*s!=NULL)
	{
		while (x<1024 && (*s!=NULL))
		{
			dishz(x,y,s,color,type);
			x+=(type+1);
			s+=2;
		}
	x=a;
	y+=(type+5);
	}
	fclose(hzk_p);
}
void open_hzk(char * path)
{
	hzk_p=fopen(path,"rb");
	if(hzk_p==NULL)
	{
		SET_SVGA_MODE(0x03);
		printf("HZK OPEN error!");
		getch();
		exit(1);
	}
}
void get_hz(char incode[],char bytes[],int type)
{
	unsigned char qh,wh;
	unsigned long offset;
	unsigned long size;
	size=type*type/8;
	qh=incode[0]-0xa0; /*�õ�����*/
	wh=incode[1]-0xa0; /*�õ�λ��*/
	offset=(94*(qh-1)+(wh-1))*size; /*�õ�ƫ��λ��*/
	fseek(hzk_p,offset,SEEK_SET); /*���ļ�ָ�뵽Ҫ��ȡ�ĺ�����ģ��*/
	fread(bytes,size,1,hzk_p); /*��ȡ32 �����ֵĺ�����ģ*/
}
void dishz(int x0,int y0,char code[],int color,int type)
{
	unsigned char mask[]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
/*������ģÿ�и�λ������*/
	int i,j,x,y,pos;
	int t;
	unsigned short far *video_buffer=(unsigned short far *)0xA0000000L;
    long pos2;
    unsigned int oldpage,newpage;  
	switch(type)
	{
		case 16:
			get_hz(code,mat0,type);
			y=y0;
			for (i=0;i<type;++i)
			{
				x=x0;
				pos=type/8*i;
				for(j=0;j<type;++j)
				{
/*���γ�������ģ��һ��λ����һ������1 ����ʾ��������ʾ�õ�*/
					if ((mask[j%8]&mat0[pos+j/8])!=NULL)
					{
						pos2=(y*800L+x);
						newpage=pos2*2/65536L;
						if(newpage!=oldpage)//�Ƿ�ҳ 
						{
							selectpage(newpage);
							oldpage=newpage;
						}
						*((unsigned short far *)(video_buffer+pos2))=color;
					}
					++x;
				}
				++y;
			}
			break;
		case 24:
			get_hz(code,mat1,type);
			y=y0;
			for (i=0;i<type;++i)
			{
				x=x0;
				pos=type/8*i;
				for(j=0;j<type;++j)
				{
/*���γ�������ģ��һ��λ����һ������1 ����ʾ��������ʾ�õ�*/
					if ((mask[j%8]&mat1[pos+j/8])!=NULL)
					{
						pos2=(y*800L+x);
						newpage=pos2*2/65536L;
						if(newpage!=oldpage)//�Ƿ�ҳ 
						{
							selectpage(newpage);
							oldpage=newpage;
						}
						*((unsigned short far *)(video_buffer+pos2))=color;
					}
				++x;
				}
				++y;
			}
			break;
	}
}
/*С���������������Ӣ�� x,yλ�ã��ַ�������ɫ*/
void hanzi12(int x,int y,char *s,int color)
{
   FILE *fp=NULL;
   char buffer[24],word[2]={'\0','\0'};
   register i,j,k;
   unsigned char qh,wh;
   unsigned long location;
   //settextstyle(0,0,1);
   if((fp=fopen("hzk12","rb"))==NULL)
   {
       printf("Can't open hzk12!");
       getch();
       exit(0);
   }
   while(*s)
   {
     if((int)(*s)>0&&(int)(*s)<128)
     {
		  word[0]=*s;
		  put_asc(x,y-2,1,1,s,color);
		  //word256(x,y+2,*s,color,color,0);  //using one is enough
          s+=1;
          x+=8;
     }
     else
     {
          qh=*s-0xa0;
          wh=*(s+1)-0xa0;
          location=(94*(qh-1)+(wh-1))*24L;
          fseek(fp,location,SEEK_SET);
          fread(buffer,24,1,fp);
          for(i=0;i<12;i++)
               for(j=0;j<2;j++)
                	for(k=0;k<8;k++)
                 		if((buffer[i*2+j]>>(7-k))&0x1)
                      		drawpoint(x+8*j+k,y+i,color);
          s+=2;
          x+=12;
       }
	}
	fclose(fp);
}