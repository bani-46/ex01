#include "token-list.h"

int cbuf;//todo
int line_num=0;

char string_attr[MAXSTRSIZE];
int num_attr;

FILE *fp;

int init_scan(char *filename){
	fp = fopen(filename,"r");
	if(fp == NULL) {
		return -1;
	} else {
		//1文字先読み
		cbuf = fgetc(fp);
		if(cbuf == EOF){

		}
		return 0;
	}
}
int scan(void){
	int i=0;

	/*for debug*/
//	static int count;
//	count++;
//	printf("%d\n",count);

	/*end of line*/
	if(cbuf == '\r'){
		cbuf = fgetc(fp);
		if(cbuf =='\n')cbuf = fgetc(fp);
		printf("Find end of line.\n");
		line_num++;
	}
	else if(cbuf == '\n'){//todo || cbuf == '\r\n' || cbuf == '\n\r'
		cbuf = fgetc(fp);
		if(cbuf =='\r')cbuf = fgetc(fp);
		printf("Find end of line.\n");
		line_num++;
	}

	/*Jump read*/
	while(cbuf == ' ' || cbuf == '\t'){
		printf("Find SP or Tab.\n");
		cbuf = fgetc(fp);
	}

	/*name*/
	if((cbuf >= 'A' && cbuf <= 'Z') || (cbuf >= 'a' && cbuf <= 'z')){
		while((cbuf >= '0' && cbuf <= '9') || (cbuf >= 'A' && cbuf <= 'Z') || (cbuf >= 'a' && cbuf <= 'z')){
			string_attr[i] = cbuf;
			i++;
			if(i > MAXSTRSIZE)error("stringsize reached MAXSTRSIZE.\n");
			cbuf = fgetc(fp);
		}
		/*judge keyword*/
		for(i = 0;i < KEYWORDSIZE;i++){
			if(strcmp(string_attr,key[i].keyword) == 0){
				return key[i].keytoken;
			}
		}
		return TNAME;
	}
	/*numbers*/
	else if(cbuf >= '0' && cbuf <= '9'){
		while(cbuf >= '0' && cbuf <= '9'){
			string_attr[i] = cbuf;
			i++;
			cbuf = fgetc(fp);
		}
		num_attr = atoi(string_attr);
		return TNUMBER;
	}
	/*symbols*/
	else{
		switch(cbuf){
		case '+':cbuf=fgetc(fp);return TPLUS;break;
		case '-':cbuf=fgetc(fp);return TMINUS;break;
		case '*':cbuf=fgetc(fp);return TSTAR;break;
		case '=':cbuf=fgetc(fp);return TEQUAL;break;
		case '<':
			cbuf=fgetc(fp);
			if(cbuf == '>'){
				cbuf=fgetc(fp);
				return TNOTEQ;
			}
			else if(cbuf == '='){
				cbuf=fgetc(fp);
				return TLEEQ;
			}
			else return TLE;
			break;
		case '>':
			cbuf=fgetc(fp);
			if(cbuf == '='){
				cbuf=fgetc(fp);
				return TGREQ;
			}
			else return TGR;
			break;
		case '(':cbuf=fgetc(fp);return TLPAREN;break;
		case ')':cbuf=fgetc(fp);return TRPAREN;break;
		case '[':cbuf=fgetc(fp);return TLSQPAREN;break;
		case ']':cbuf=fgetc(fp);return TRSQPAREN;break;
		case '.':cbuf=fgetc(fp);return TDOT;break;
		case ',':cbuf=fgetc(fp);return TCOMMA;break;
		case ':':
			cbuf=fgetc(fp);
			if(cbuf == '='){
				cbuf=fgetc(fp);
				return TASSIGN;
			}
			else return TCOLON;
			break;
		case ';':cbuf=fgetc(fp);return TSEMI;break;
		default:
					return -1;
		}
	}
	/*
	 * EOFに注意
	 */
}

int get_linenum(void){
	return line_num;
}
void end_scan(void){
	fclose(fp);
	printf("[INFO]Close File.\n");
}
