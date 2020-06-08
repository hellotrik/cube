#define DRIVER
#define SHOWSTATS
#define DUMP
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#ifndef HUFFMAN_H
#define MAXSYMBOLS 256
#define MAXNODES ((MAXSYMBOLS<<1)-1)
#define LEFT 0
#define RIGHT 1
typedef struct{int index,child[2];} Header;
#define SIGNATURE "Huff1"

#endif

#define STATS(x) x
#if defined(SHOWSTATS)
unsigned long HuffBytesHdr,HuffBytesOut;
#endif
struct{
	unsigned long count;
	int child[2],parent,bitcount;
	char *hcode;
} HuffTree[MAXNODES];

unsigned long HuffBytesIn,HuffBytes;
int HuffRoot,HuffCount;
char *HuffTable=NULL,*HuffTablePtr;

#ifdef DUMP
static void HuffDump(int n){
	int i;
	printf("root is %d\n",HuffRoot);
	printf("No. Sym Count Parent   "
		   "Left Right BitNo Bits\n");
	for(i=0;i<=n;++i){
		if(HuffTree[i].count){
			printf("%3d. ",i);
			if(i<MAXSYMBOLS){
				if(isprint(i))printf("%c",i);
				else printf("x%.2x",i);
			}else printf("n/a");
			printf("%71d %5d %5d %5d",
					HuffTree[i].count,
					HuffTree[i].parent,
					HuffTree[i].child[LEFT],
					HuffTree[i].child[RIGHT]);
			if(HuffTree[i].hcode){
				int j,k,byte,bit;
				j=HuffTree[i].bitcount;
				printf("  %5d ",j);
				for(k=0;k<j;++k){
					byte=k>>3;
					bit=k-(byte<<3);
					if(HuffTree[i].hcode[byte]&(1<<(7-bit)))
						printf("1");
					else printf("0");
				}
			}
			printf("\n");
		}
	}
	printf("\n");
}

#define DUMPTREE(x) HuffDump(x)
#else
#define DUMPTREE(x)
#endif

static int HuffScan(FILE *infile){
	int c;
	HuffBytesIn=0;
	while((c=fgetc(infile))!=EOF){
		++HuffTree[c].count;
		++HuffBytesIn;
	}
	if(ferror(infile))return 1;
	else
	{
		rewind(infile);return 0;
	}
}

static void HuffWalk(int k,int depth){
	int dir,bitno,set,byte,bit, w,trace;
	for(dir=0;dir<2;++dir)
	{
		if(HuffTree[k].child[dir]==-1)
		{
			if(!dir){
				if(!HuffTablePtr)HuffBytes+=(depth+7)>>3;
				else
				{
					HuffTree[k].bitcount=depth;
					HuffTree[k].hcode=HuffTablePtr;
					bitno=depth-1;
					for(trace=k,w=HuffTree[k].parent;w;
						trace=w,w=HuffTree[w].parent)
					{
						set=(HuffTree[w].child[LEFT]==trace)?LEFT:RIGHT;
						if(set){
							byte=bitno>>3;
							bit=bitno-(byte<<3);
							HuffTablePtr[byte]|=1<<(7-bit);
						}
						--bitno;
					}
					HuffTablePtr+=(depth+7)>>3;
					
				}
			}
		}
		else HuffWalk(HuffTree[k].child[dir],depth+1);
	}
}

static int HuffBuild(void){
	int i,k,lo1,lo2;
	for(i=0;i<MAXSYMBOLS;++i)if(HuffTree[i].count)++HuffCount;
	if(HuffCount<1)return 2;
	if(HuffCount==1){
		for(i=0;i<MAXSYMBOLS;++i)
		if(HuffTree[i].count==0){
			++HuffTree[i].count;
			++HuffCount;
			break;
		}
	}
	HuffRoot=MAXSYMBOLS+HuffCount-2;
	for(i=MAXSYMBOLS;i<=HuffRoot;++i){
		for(k=0;k<i;++k){
			if(HuffTree[k].count&&!HuffTree[k].parent){
				lo1=k++;break;
			}
		}
		for(;k<i;++k){
			if(HuffTree[k].count&&!HuffTree[k].parent){
				lo2=k++;break;
			}
		}
		for(;k<i;++k){
			if(HuffTree[k].count&&!HuffTree[k].parent){
				if(HuffTree[k].count<HuffTree[lo1].count)lo1=k;
				else if(HuffTree[k].count<HuffTree[lo2].count)lo2=k;
			}
		}
		HuffTree[i].count=HuffTree[lo1].count+HuffTree[lo2].count;
		HuffTree[i].child[LEFT]=lo1;
		HuffTree[i].child[RIGHT]=lo2;
		HuffTree[lo1].parent=HuffTree[lo2].parent=i;
	}
	HuffBytes=0;
	HuffTablePtr=NULL;
	HuffWalk(HuffRoot,0);
	HuffTable=(char*)calloc(HuffBytes,sizeof(char));
	if(!HuffTable)return 3;
	HuffTablePtr=HuffTable;
	HuffWalk(HuffRoot,0);
	return 0;	
}


static int HuffCompress(FILE *infile,FILE *outfile){
	int i;
	Header header;
	int outchar,outbit,c;
	char sig[]=SIGNATURE;
	HuffBytesHdr=HuffBytesOut=0;
	size_t sz=fwrite(sig,1,strlen(sig)+1,outfile);
	HuffBytesHdr+=sz;
	
	fwrite(&HuffRoot,sizeof(HuffRoot),1,outfile);
	HuffBytesHdr+=sizeof(HuffRoot);
	
	fwrite(&HuffCount,sizeof(HuffCount),1,outfile);
	HuffBytesHdr+=sizeof(HuffCount);
	
	fwrite(&HuffBytesIn,sizeof(HuffBytesIn),1,outfile);
	HuffBytesHdr+=sizeof(HuffBytesIn);
	printf("%d,%d,%d,%d\n",sz,HuffRoot,HuffCount,HuffBytesIn);
	for(i=0;i<=HuffRoot;++i){
		if(HuffTree[i].count){
			header.index=i;
			header.child[0]=HuffTree[i].child[0];
			header.child[1]=HuffTree[i].child[1];
			fwrite(&header,sizeof(header),1,outfile);
			HuffBytesHdr+=sizeof(header);
		}
	}
	outchar=0;
	outbit=0;
	while((c=fgetc(infile))!=EOF){
		char *s;
		int k,count,byte,bit,set;
		s=HuffTree[c].hcode;
		count=HuffTree[c].bitcount;
		for(k=0;k<count;++k){
			byte=k>>3;
			bit=k-(byte<<3);
			set=(s[byte]&(1<<(7-bit)))?1:0;
			if(set) outchar|=1<<(7-outbit);
			++outbit;
			if(outbit==8){
				fputc(outchar,outfile);
				outchar=outbit=0;
				++HuffBytesOut;
			}
		}
	}
	
	if(outbit){
		fputc(outchar,outfile);
		++HuffBytesOut;
	}
	if(ferror(infile))return 1;
	if(ferror(outfile))return 4;
	return 0;	
}

int HuffEncode(FILE *infile,FILE* outfile){
	int retval,i;
	HuffRoot=HuffCount=0;
	memset(HuffTree,0,sizeof(HuffTree));
	for(i=0;i<MAXSYMBOLS;++i){
		HuffTree[i].child[LEFT]=HuffTree[i].child[RIGHT]=-1;
	}
	if(retval=HuffScan(infile)) goto done;
	if(retval=HuffBuild()) goto done;
	DUMPTREE(HuffRoot);
	retval=HuffCompress(infile,outfile);
	#if defined(SHOWSTATS)
	if(!retval){
		printf("The input file contained %lu bytes\n",HuffBytesIn);
		printf("The output file contained %lu header bytes"
				" and %lu data bytes\n",HuffBytesHdr,HuffBytesOut);
		printf("Output file %lu%% the size of input file\n"
				,(HuffBytesHdr+HuffBytesOut)*100/HuffBytesIn);		
	}
	#endif
done:
	fclose(infile);
	fclose(outfile);
	return retval;
}

int HuffDecode(FILE*infile,FILE*outfile){
	int retval=0,i;
	char buffer[10];
	Header header;
	unsigned long bytesout;
	int inbyte,bitno,mask,set;
	memset(HuffTree,0,sizeof(HuffTree));
	fgets(buffer,strlen(SIGNATURE)+2,infile);
	if(strcmp(buffer,SIGNATURE)){
		retval=1;goto done;
	}
	fread(&HuffRoot,sizeof(HuffRoot),1,infile);
	fread(&HuffCount,sizeof(HuffCount),1,infile);
	fread(&HuffBytesIn,sizeof(HuffBytesIn),1,infile);
	printf("%d,%d,%d,%d\n",strlen(SIGNATURE)+2,HuffRoot,HuffCount,HuffBytesIn);
	for(i=0;i<HuffCount+(HuffRoot-MAXSYMBOLS+1);++i){
		fread(&header,sizeof(header),1,infile);
		HuffTree[header.index].child[0]=header.child[0];
		HuffTree[header.index].child[1]=header.child[1];
	}
	if(ferror(infile)){
		retval=2;goto done;
	}
	DUMPTREE(HuffRoot);
	
	bitno=8;
	for(bytesout=0;bytesout<HuffBytesIn;++bytesout){
		i=HuffRoot;
		for(;;){
			if(bitno>7)
			{
				inbyte=fgetc(infile);
				if(inbyte==EOF){
					retval=3;goto done;
				}
				bitno=0;
				mask=0x80;
			}
			set=(inbyte&mask)?1:0;
			++bitno;
			mask>>=1;
			i=HuffTree[i].child[set];
			if(HuffTree[i].child[0]==-1){
				fputc(i,outfile);break;
			}
		}
	}
done:
	printf("ret:%d\n",retval);
	fclose(infile);
	fclose(outfile);
	return retval;
}

#ifdef DRIVER
int main(int argc,char**argv)
{
	FILE *infile,*outfile;
	int retval;
	if(argc!=4)return 1;
	infile=fopen(argv[2],"rb");
	outfile=fopen(argv[3],"wb");
	if(infile==NULL||outfile==NULL)return 1;
	if(strcmp(argv[1],"-e")==0){
		if(retval=HuffEncode(infile,outfile)){
			printf("compression faild: ");
		
		}else{
			printf("success\n");
			return 0;
		}
	}else{
		if(retval=HuffDecode(infile,outfile)){
			printf("decompression faild: %d",retval);
			return retval;
		}else{
			printf("%s was expanded into %s\n",argv[2],argv[3]);
			return 0;
		}
	}
}
#endif









