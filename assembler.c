#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

int amount_label(FILE * p,int n,int counter){
    rewind(p);char line[30];
    int i=0;
    for(;i<n-1;i++)
        fscanf(p,"%[^\n]%*c",line);
    fscanf(p,"%s",line);
    fscanf(p,"%s",line);
    int amount=0;
    fscanf(p,"%s",line);
    int k=0;
    while(line[k]>=48&&line[k]<=57){
        amount+=line[k]-48;
        amount*=10;
        k++;
    }
    rewind(p);
    i=0;
    for(;i<counter;i++)
        fscanf(p,"%[^\n]%*c",line);
    return amount;

}
int find_labal(char labal[][7],char name[],int counter){
    int i=0;
    for(;i<counter;i++)
        if(strcmp(labal[i],name)==0)
            return i;
    return -1;
}
int duplicate(char p[][7],int counter){
    int i=0;
    for(;i<counter-2;i++){
        int j=i+1;
        for(;j<counter;j++){
            if(strcmp(p[i],p[j])==0&&((p[i][0]>=65&&p[i][0]<=90)||(p[i][0]>=97&&p[i][0]<=122))&&
                    ((p[j][0]>=65&&p[j][0]<=90)||(p[j][0]>=97&&p[j][0]<=122))){
                printf("duplicate 1 %s\nduplicate 2 %s",p[i],p[j]);
                return 1;
            }
            else
                continue;
        }
    }
    return 0;
}
struct R_type{
    unsigned int opcode;    //27-24
    unsigned int rd;        //15-12
    unsigned int rt;        //19-16
    unsigned int rs;        //23-20
    int unused1;    //28-31
    int unused2;    //11-0

};
void printR(struct R_type a){
    int r=0;r+=a.opcode*pow(16,6);
    r+=a.rs*pow(16,5);r+=a.rt*pow(16,4);r+=a.rd*pow(16,3);
    printf("%d\n",r);}
void fprintR(struct R_type a ,FILE * assm){

    int r=0;r+=a.opcode*pow(16,6);
    r+=a.rs*pow(16,5);r+=a.rt*pow(16,4);r+=a.rd*pow(16,3);
    fprintf(assm,"%d\n",r);
}
void sefrR(struct R_type a){
    a.unused1=0;
    a.unused2=0;
}
struct I_type{
    unsigned int opcode;             //27-24
    unsigned int rs;                 //23-20        در اصل اینجا اشتباه شده ولی با همین ادامه میدیم
    unsigned int rt;                //19-16
    int offset;                 //15-0
    unsigned int unused;    //31-28



};
void sefrI(struct I_type a){
    a.unused=0;
}
void printI(struct I_type a){
    int r=0;r+=a.opcode*pow(16,6);r+=a.rs*pow(16,5);
    r+=a.offset;
    if(a.offset>=pow(16,4)){
        printf("overflow occured!\n");
        exit(1);}
    printf("%d\n",r);
};
void fprintI(struct I_type a, FILE* assm)
{
    int r=0;r+=a.opcode*pow(16,6);r+=a.rs*pow(16,5);
    r+=a.rt*pow(16,4);
    r+=a.offset;
    if(a.offset>=pow(16,4)){
        printf("overflow occured!\n");
        exit(1);}
    fprintf(assm,"%d\n",r);
}
struct J_type{
    unsigned int opcode;        //27-24

    unsigned int unused1;       //23-16

    unsigned int unused2;       //31-28

    unsigned int offset;                //15-0


};
void sefrj(struct J_type a){
    a.unused1=0;
    a.unused2=0;
}
void printj(struct J_type a){
    int r=0;int of=a.offset;
    r+=(of%10);of/=10;
    r+=(of%10)*16;of/=10;
    r+=(of%10)*pow(16,2);of/=10;
    r+=(of%10)*pow(16,3);
    r+=a.opcode*pow(16,6);
    printf("%d\n",r);
};
void fprintj(struct J_type a, FILE * assm){
    int r=0;int of=a.offset;
    r+=(of%10);of/=10;
    r+=(of%10)*16;of/=10;
    r+=(of%10)*pow(16,2);of/=10;
    r+=(of%10)*pow(16,3);
    r+=a.opcode*pow(16,6);
    fprintf(assm,"%d\n",r);
}
void main(int argc,char **argv){
    FILE *assp,*machp,*fopen();
    if(argc < 3){
        printf("***** Please run this program as follows:\n");
        printf("***** %s assprog.as machprog.m\n",argv[0]);
        printf("***** where assprog.as is your assembly program\n");
        printf("***** and machprog.m will be your machine code.\n");
        exit(1);
    }
    if((assp=fopen(argv[1],"r")) == NULL){
        printf("%s cannot be openned\n",argv[1]);
        exit(1);
    }
    if((machp=fopen(argv[2],"w+")) == NULL){
        printf("%s cannot be openned\n",argv[2]);
        exit(1);
    }
    // here you can place your code for the assembler
    char labales[20]/*65536*/[7];
    int counter=0;
    char line[40];
    /*while (!feof(assp)){ */           //getting labales

    //            fscanf(assp,"%[^\n]%*c",line);
    while ((fgets(line,40,assp) )!= NULL){
        int i=0;
        for(;i<6;i++){
            if (line[i]!='\t'&&line[i]!=' ')
                labales[counter][i]=line[i];
            else
                break;
        }
        labales[counter][i]='\0';
        counter++;
    }
    if(duplicate(labales,counter)){
        puts("eror! there is duplicate labels");
        exit(1);
    }
    rewind(assp);
    int len_of_labales=counter;
    counter=-1;
    while((fgets(line,40,assp) )!= NULL && counter!=len_of_labales-2){
        counter++;
        //        printf("%d\n",counter);
        //            char line[30];
        rewind(assp);
        int i=0;
        for(;i<counter;i++)
            fscanf(assp,"%[^\n]%*c",line);
        //        puts(line);
        if((labales[counter][0]>=65&&labales[counter][0]<=90)||
                (labales[counter][0]>=97&&labales[counter][0]<=122)){
            fscanf(assp,"%s",line);
        }
        fscanf(assp,"%s",line);
        //        printf("%d\n",counter);
        //        puts(line);
        if(strcmp(line,"halt")==0||strcmp(line,"j")==0)                                 //j-type
        {
            struct  J_type cmd;
            sefrj(cmd);
            if(strcmp(line,"halt")==0){
                //                    halt=counter;
                cmd.opcode=14;
                cmd.offset=0;
                fprintj(cmd,machp);
                //                printj(cmd);
                continue;
                //                    fclose(assm);
                //                    fclose(machinecode);
                //                    return 0;
            }
            else{
                cmd.opcode=13;
                fscanf(assp,"%s",line);
                int n=find_labal(labales,line,len_of_labales);
                if(n<0)
                {
                    puts("erore 1");
                    fclose(assp);
                    fclose(machp);
                    exit(1);
                }
                //                    counter=n-1;
                cmd.offset=n;
                fprintj(cmd,machp);
                //                printj(cmd);
                continue;
            }

        }


        if(strcmp(line,"lw")==0||strcmp(line,"addi")==0
                ||strcmp(line,"ori")==0||strcmp(line,"slti")==0||strcmp(line,"sw")==0
                ||strcmp(line,"beq")==0||strcmp(line,"jalr")==0||strcmp(line,"lui")==0){    //I_type_check be joz sw
            struct I_type cmd;
            sefrI(cmd);
            switch (line[0]) {
            case 'l':
            {
                if(strcmp(line,"lw")==0)   {               //lw
                    cmd.opcode=9;
                    fscanf(assp,"%s",line);
                    int k=0;
                    int a=line[k]-48;k++;
                    if(line[k]!=44){
                        a*=10;
                        a+=line[k]-48;
                        k++;
                    }
                    k++;
                    int a1=line[k]-48;k++;
                    if(line[k]!=44){
                        a1*=10;
                        a1+=line[k]-48;
                        k++;
                    }
                    k++;
                    if(line[k]>=48&&line[k]<=57){                               //numeric address
                        int num=0;
                        while(line[k]>=48&&line[k]<=57){
                            num+=line[k]-48;
                            num*=10;
                            k++;
                        }
                        num/=10;
                        if(num<0||num>9999)
                        {
                            puts("offset is invallid .");
                            fclose(machp);
                            fclose(assp);
                            exit(1);
                        }
                        cmd.rs=a1;cmd.rt=a;
                        cmd.offset=num;
                        //                        registers[a]=amount_label(assp,registers[a1]+num,counter);
                        fprintI(cmd,machp);
                        //                        printI(cmd);
                        continue;
                    }
                    else{                                                       //logical address
                        char lbl[7];int i=0;
                        while((line[k]>=65&&line[k]<=90)||(line[k]>=97&&line[k]<=122)){
                            lbl[i]=line[k];i++;k++;
                        }
                        lbl[i]='\0';
                        cmd.rs=a1;cmd.rt=a;cmd.offset=find_labal(labales,lbl,len_of_labales);
                        if(cmd.offset<0){
                            puts("error . label does not found");
                            fclose(assp);
                            fclose(machp);
                            exit(1);
                        }
                        //                        registers[a]=amount_label(assp,cmd.offset,counter);
                        fprintI(cmd,machp);
                        //                        printI(cmd);
                        continue;


                    }

                }
                else{                              //lui
                    cmd.opcode=8;
                    cmd.rt=0;
                    fscanf(assp,"%s",line);
                    int k=0;
                    int a=0,imm=0;
                    a+=line[k]-48;
                    k++;
                    if(line[k]!=44)
                    {
                        a*=10;
                        a+=line[k]-48;
                        k++;
                    }
                    k++;
                    while (line[k]>=48&&line[k]<=57) {
                        imm+=line[k]-48;
                        imm*=10;
                        k++;
                    }
                    imm/=10;
                    cmd.offset=imm;
                    cmd.rs=a;
                    //                    registers[a]=imm*pow(2,16);
                    fprintI(cmd,machp);
                    //                    printI(cmd);
                    continue;
                }
            }break;
            case 'a':                           //addi
            {
                cmd.opcode=5;
                fscanf(assp,"%s",line);
                int k=0;
                int a=line[k]-48;k++;
                if(line[k]!=44){
                    a*=10;
                    a+=line[k]-48;
                    k++;
                }
                k++;
                int a1=line[k]-48;k++;
                if(line[k]!=44){
                    a1*=10;
                    a1+=line[k]-48;
                    k++;
                }
                k++;
                int num=0;
                while(line[k]>=48&&line[k]<=57){
                    num+=line[k]-48;
                    num*=10;
                    k++;
                }
                num/=10;
                cmd.rs=a1;cmd.rt=a;cmd.offset=num;
                //                registers[a]=registers[a1]+num;
                fprintI(cmd,machp);
                //                printI(cmd);
                continue;

            }break;
            case 'o':{                          //ori
                cmd.opcode=7;
                fscanf(assp,"%s",line);
                int k=0;
                int a=line[k]-48;k++;
                if(line[k]!=44){
                    a*=10;
                    a+=line[k]-48;
                    k++;
                }
                k++;
                int a1=line[k]-48;k++;
                if(line[k]!=44){
                    a1*=10;
                    a1+=line[k]-48;
                    k++;
                }
                k++;
                int num=0;
                while(line[k]>=48&&line[k]<=57){
                    num+=line[k]-48;
                    num*=10;
                    k++;
                }
                num/=10;
                cmd.rt=a;cmd.rs=a1;cmd.offset=num;
                //                registers[a]=registers[a1]|num;
                fprintI(cmd,machp);
                /*       printI(cmd);*/continue;
            }break;
            case 's':{
                if(strcmp(line,"sw")==0){       //sw
                    cmd.opcode=10;
                    fscanf(assp,"%s",line);
                    int k=0;
                    int a=line[k]-48;k++;
                    if(line[k]!=44){
                        a*=10;
                        a+=line[k]-48;
                        k++;
                    }
                    k++;
                    int a1=line[k]-48;k++;
                    if(line[k]!=44){
                        a1*=10;
                        a1+=line[k]-48;
                        k++;
                    }
                    k++;
                    if(line[k]>=48&&line[k]<=57){                               //numeric address
                        int num=0;
                        while(line[k]>=48&&line[k]<=57){
                            num+=line[k]-48;
                            num*=10;
                            k++;
                        }
                        num/=10;
                        if(num<0||num>9999)
                        {
                            puts("offset is invallid .");
                            fclose(machp);
                            fclose(assp);
                            exit(1);
                        }
                        cmd.rs=a1;cmd.rt=a;
                        cmd.offset=num;
                        //                        registers[a]=amount_label(assp,registers[a1]+num,counter);
                        fprintI(cmd,machp);
                        //                        printI(cmd);
                        continue;
                    }
                    else{                                                       //logical address
                        char lbl[7];int i=0;
                        while((line[k]>=65&&line[k]<=90)||(line[k]>=97&&line[k]<=122)){
                            lbl[i]=line[k];i++;k++;
                        }
                        lbl[i]='\0';
                        cmd.rs=a1;cmd.rt=a;cmd.offset=find_labal(labales,lbl,len_of_labales);
                        if(cmd.offset<0){
                            puts("error . label does not found");
                            fclose(assp);
                            fclose(machp);
                            exit(1);
                        }
                        //                        registers[a]=amount_label(assp,cmd.offset,counter);
                        //                        printI(cmd);

                        cmd.rs=a1;cmd.rt=a;cmd.offset=find_labal(labales,lbl,len_of_labales);
                        fprintI(cmd,machp);
                        //                    printI(cmd);
                        continue;}
                }
                else{                           //slti
                    cmd.opcode=6;
                    fscanf(assp,"%s",line);
                    int k=0;
                    int a=line[k]-48;k++;
                    if(line[k]!=44){
                        a*=10;
                        a+=line[k]-48;
                        k++;
                    }
                    k++;
                    int a1=line[k]-48;k++;
                    if(line[k]!=44){
                        a1*=10;
                        a1+=line[k]-48;
                        k++;
                    }
                    k++;
                    int num=0;
                    while(line[k]>=48&&line[k]<=57){
                        num+=line[k]-48;
                        num*=10;
                        k++;
                    }
                    num/=10;
                    cmd.offset=num;
                    cmd.rs=a1;cmd.rt=a;
                    //                    if(registers[a1]<num)
                    //                        registers[a]=1;
                    //                    else
                    //                        registers[a]=0;
                    fprintI(cmd,machp);
                    /*  printI(cmd);*/continue;
                }

            }break;
            case 'b':{                          //beq
                cmd.opcode=11;
                fscanf(assp,"%s",line);
                //                printf("line %s\n",line);
                int k=0;
                int a=line[k]-48;k++;
                if(line[k]!=44){
                    a*=10;
                    a+=line[k]-48;
                    k++;
                }
                k++;
                int a1=line[k]-48;k++;
                if(line[k]!=44){
                    a1*=10;
                    a1+=line[k]-48;
                    k++;
                }
                k++;
                char lbl[7];int i=0;
                while((line[k]>=65&&line[k]<=90)||(line[k]>=97&&line[k]<=122)){
                    lbl[i]=line[k];i++;k++;
                }
                lbl[i]='\0';int tmp_of_ofset;
                cmd.rt=a1;cmd.rs=a;tmp_of_ofset=find_labal(labales,lbl,len_of_labales);

                if(tmp_of_ofset<0){
                    puts("error . label does not found");
                    fclose(assp);
                    fclose(machp);
                    exit(1);
                }
                //                if(registers[a]==registers[a1])
                //                    counter=cmd.offset-1;
                cmd.offset=tmp_of_ofset-counter-1;
                //                printf("rt= %d \nrs= %d \n offset ya lable = %d\n",cmd.rt,cmd.rs,cmd.offset);
                fprintI(cmd,machp);
                //                printI(cmd);
                continue;

            }break;
            default:{                           //jalr
                cmd.opcode=12;
                cmd.offset=0;
                fscanf(assp,"%s",line);
                int k=0;
                int a=line[k]-48;k++;
                if(line[k]!=44){
                    a*=10;
                    a+=line[k]-48;
                    k++;
                }
                k++;
               int a1=line[k]-48;k++;
                if(line[k]>=48 && line[k]<=57 ){
                    a1*=10;
                    a1+=line[k]-48;
                    k++;
                }

                cmd.rt=a;cmd.rs=a1;
                //                registers[a]=counter+1;
                //                    counter=registers[a1]-1;
                fprintI(cmd,machp);
                //                printI(cmd);
                continue;

            }
            }

        }


        if(strcmp(line,"add")==0||strcmp(line,"sub")==0||
                strcmp(line,"slt")==0||strcmp(line,"or")==0||
                strcmp(line,"nand")==0){                                                    //R_type_check
            struct R_type cmd;
            sefrR(cmd);
            switch (line[0]) {
            case 'a':{                                                  //add
                cmd.opcode=0;
                fscanf(assp,"%s",line);
                int k=0;
                int a=line[k]-48;k++;
                if(line[k]!=44){
                    a*=10;
                    a+=line[k]-48;
                    k++;
                }
                k++;
                int a1=line[k]-48;k++;
                if(line[k]!=44){
                    a1*=10;
                    a1+=line[k]-48;
                    k++;
                }
                k++;
                int a2=line[4]-48;k++;
                if(line[k]>=48&&line[k]<=57)
                {
                    a2*=10;
                    a2+=line[k]-48;
                }
                //                registers[a]=registers[a1]+registers[a2];
                cmd.rd=a;cmd.rt=a2;cmd.rs=a1;
                fprintR(cmd,machp);
                /*  printR(cmd);*/continue;
            }break;
            case 's':{
                if(strcmp(line,"slt")==0){                              //slt
                    cmd.opcode=2;
                    fscanf(assp,"%s",line);int k=0;
                    int a=line[k]-48;k++;
                    if(line[k]!=44){
                        a*=10;
                        a+=line[k]-48;
                        k++;
                    }
                    k++;
                    int a1=line[k]-48;k++;
                    if(line[k]!=44){
                        a1*=10;
                        a1+=line[k]-48;
                        k++;
                    }
                    k++;
                    int a2=line[4]-48;k++;
                    if(line[k]>=48&&line[k]<=57)
                    {
                        a2*=10;
                        a2+=line[k]-48;
                    }
                    //                    if(registers[a1]<registers[a2])
                    //                        registers[a]=1;
                    //                    else
                    //                        registers[a]=0;
                    cmd.rd=a;cmd.rs=a1;cmd.rt=a2;
                    fprintR(cmd,machp);
                    /*  printR(cmd);*/continue;
                }
                else{                                                   //sub
                    cmd.opcode=1;
                    fscanf(assp,"%s",line);
                    int k=0;
                    int a=line[k]-48;k++;
                    if(line[k]!=44){
                        a*=10;
                        a+=line[k]-48;
                        k++;
                    }
                    k++;
                    int a1=line[k]-48;k++;
                    if(line[k]!=44){
                        a1*=10;
                        a1+=line[k]-48;
                        k++;
                    }
                    k++;
                    int a2=line[4]-48;k++;
                    if(line[k]>=48&&line[k]<=57)
                    {
                        a2*=10;
                        a2+=line[k]-48;
                    }
                    //                    registers[a]=registers[a1]-registers[a2];
                    cmd.rd=a;cmd.rt=a2;cmd.rs=a1;
                    fprintR(cmd,machp);
                    //                    printR(cmd);
                    continue;
                }
            }break;
            case 'o':{                                                  //or
                cmd.opcode=3;
                fscanf(assp,"%s",line);
                int k=0;
                int a=line[k]-48;k++;
                if(line[k]!=44){
                    a*=10;
                    a+=line[k]-48;
                    k++;
                }
                k++;
                int a1=line[k]-48;k++;
                if(line[k]!=44){
                    a1*=10;
                    a1+=line[k]-48;
                    k++;
                }
                k++;
                int a2=line[4]-48;k++;
                if(line[k]>=48&&line[k]<=57)
                {
                    a2*=10;
                    a2+=line[k]-48;
                }
                //                registers[a]=registers[a1]|registers[a2];
                cmd.rd=a;cmd.rs=a1;cmd.rt=a2;
                fprintR(cmd,machp);
                /*printR(cmd);*/continue;
            }break;
            default:{                                                   //nand
                cmd.opcode=4;
                fscanf(assp,"%s",line);
                int k=0;
                int a=line[k]-48;k++;
                if(line[k]!=44){
                    a*=10;
                    a+=line[k]-48;
                    k++;
                }
                k++;
                int a1=line[k]-48;k++;
                if(line[k]!=44){
                    a1*=10;
                    a1+=line[k]-48;
                    k++;
                }
                k++;
                int a2=line[4]-48;k++;
                if(line[k]>=48&&line[k]<=57)
                {
                    a2*=10;
                    a2+=line[k]-48;
                }
                //                registers[a]=~(registers[a1]&registers[a2])+1;
                cmd.rd=a;cmd.rs=a1;cmd.rt=a2;
                fprintR(cmd,machp);
                /*    printR(cmd);*/continue;
            }
            }

        }



        if(strcmp(line,".fill")==0){                                      // check
            fscanf(assp,"%s",line);
            int n=0;
            int k=0;
            bool manfi=false;
            if(line[0]==45){
                manfi=true;k++;}
            if(line[k]>=48&&line[k]<=57){
                while(line[k]>=48&&line[k]<=57){
                    n+=line[k]-48;
                    n*=10;
                    k++;
                }
                n/=10;
                if(manfi)
                    n*=-1;
                fprintf(machp,"%d\n",n);
                //                printf("fill ast %d\n",n);
            }
            else{                       //labales
                char lbl[7];int i=0;
                while((line[k]>=65&&line[k]<=90)||(line[k]>=97&&line[k]<=122)){
                    lbl[i]=line[k];i++;k++;
                }
                lbl[i]='\0';
                //                printf("onney ke mikham %s\n",lbl);
                int n=find_labal(labales,lbl,len_of_labales);
                if(n<0){
                    puts("error . label does not found");
                    fclose(assp);
                    fclose(machp);
                    exit(1);
                }
                fprintf(machp,"%d\n",n);
                //                printf("fill asst %d\n",n);
            }
        }


        else if(counter>len_of_labales-1){
            puts("opcode is invalid!");
            printf("%s\ncoutner is %d\n",line,counter);
            exit(1);}
        //            counter++;
    }
    fclose(assp);
    fclose(machp);
    exit(0);
}
