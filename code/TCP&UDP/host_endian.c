#include <stdio.h>
union endian{
    short number;
    char b[2];
}obj;

int main(){
    obj.number=0x1234;
    if(obj.b[0]==0x34){
        printf("0x1234在主机中的存储顺序为0x3412, 为little endian")
    }
    else{
        printf("0x1234在主机中的存储顺序为0x1234, 为big endian")
    }
    //fprintf(stderr,"%x\n",obj.b[0]);
}