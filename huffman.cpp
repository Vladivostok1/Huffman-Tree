#include <iostream>
#include <fstream>
#include <algorithm>
#include <queue>
#include <stack>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <windows.h>

using namespace std;

typedef struct HTNode{
    long weight;
    char code;
    struct HTNode *parent,*lchild,*rchild;
}HTNode,*HuffmanTree;

typedef struct{
    char code;
    long times;
}StatisticsNode;

void HuffmanDecode(char *FileName);
void HuffmanCode(char *FileName);
HuffmanTree CreateHuffmanTree(StatisticsNode *TN,int n);
bool CompareTimeNode(StatisticsNode a,StatisticsNode b);
bool CompareHTNode(HuffmanTree a,HuffmanTree b);
void ShowHuffmanTree(HuffmanTree HT);
void CreateHuffmanCode(char **Code,HuffmanTree HT,char *LastCode);
int ChangeCodeToChar(HuffmanTree HT,queue<char> &MyQueue);

int main(int argc,char *argv[])
{

    if (argc >= 1){
        if (argc == 1){
            cout <<"20170333"<< "帮助说明：" << endl << "HuffmanCoder -code 文件名:        压缩文件" << endl << "HuffmanCoder -decode 文件名:        解压缩文件" << endl;
            system("pause");
        }
        else{
            if (strcmp(argv[1],"-code") == 0){
                HuffmanCode(argv[2]);
            }
            else if (strcmp(argv[1],"-decode") == 0){
                HuffmanDecode(argv[2]);
            }
            else{
                cout << "参数错误！" << endl;
                system("pause");
            }
        }

    }
    else{
        cout << "参数错误！" << endl;
    }

    //HuffmanDecode("test.cod");
    /*
    int MenuId = 0;
    while (true){
        cout << "1.文件编码" << endl << "2.文件译码" << endl << "3.退出" << endl;
        cout << "请输入命令:" << endl;
        cin >> MenuId;
        if (MenuId == 1){
            char TempName[256];
            cout << "请输入文件名：" << endl;
            cin >> TempName;
            HuffmanCode(TempName);
        }
        else if (MenuId == 2){
            char TempName[256];
            cout << "请输入文件名：" << endl;
            cin >> TempName;
            HuffmanDecode(TempName);
        }
        else if (MenuId == 3){
            return 0;
        }
        else{
            cout << "输入未知指令。" << endl;
        }
    }
*/
    return 0;
}

void HuffmanDecode(char *FileName){
    ifstream FileIn(FileName,ios::binary);
    char *TempFileName = new char[strlen(FileName) + 1];
    char Sign[3];
    FileIn.read(Sign,2);
    Sign[2] = '\0';
    //cout << Sign << endl;
    if (strcmp(Sign,"HC") != 0){
        cout << "不是合法的文件！" << endl;
        return;
    }
   
    FileIn.read(TempFileName,strlen(FileName) + 1);

    ofstream FileOut(TempFileName,ios::binary);

    int n;
    FileIn.read((char *)&n,sizeof(n));
    //cout << "n:" << n << endl;
    //system("pause");
    StatisticsNode TimeNode[256];

    FileIn.read((char *)&TimeNode,sizeof(StatisticsNode) * n);
   
    long FileLength;
    FileIn.read((char *)&FileLength,sizeof(FileLength));
    //cout << "FileLength:" << FileLength << endl;

    HuffmanTree NewHT = CreateHuffmanTree(TimeNode,n);

    //ShowHuffmanTree(NewHT);
    //system("pause");
    queue<char> MyQueue;
    stack<char> TempStack;

   
    char ch;
    //cout << FileIn.get() << endl;
    while (!FileIn.eof()){
        ch = FileIn.get();
        int k;
        //cout << ch << endl;
        k = (unsigned char)ch;
        //cout << k << endl;

        int xi;
        while (k != 0){
            char tempc;
            tempc = k % 2 + '0';
            TempStack.push(tempc);
            k = k / 2;
        }

    
        while (TempStack.size() < 8){
            TempStack.push('0');
        }

 
        while (!TempStack.empty()){
            //cout << TempStack.top() << " ";
            MyQueue.push(TempStack.top());
            TempStack.pop();
        }
        //cout << endl;

        while (MyQueue.size() >= 32){
            int tempch;
            tempch = ChangeCodeToChar(NewHT,MyQueue);
            //cout << "size:" << MyQueue.size() << endl;
            //cout << "tempch:" << (int)(unsigned char)tempch << endl;
            FileOut << (char)tempch;
        }
    }
    
    //cout << "tail:" << endl;
    while (MyQueue.size() >= 8){
        int tempch;
        tempch = ChangeCodeToChar(NewHT,MyQueue);
        //cout << "tempch:" << (int)(unsigned char)tempch << endl;
        if (tempch == 256){
            break;
        }
        FileOut << (char)tempch;
        if (FileOut.tellp() == FileLength){
            break;
        }
    }

    delete TempFileName;
    FileIn.close();
    FileOut.close();
    cout << "解压完成！" << endl;
    //cout << TempFileName << endl;
    //ofstream FileOut(TempFileName,ios::binary | ios::trunc);
}

int ChangeCodeToChar(HuffmanTree HT,queue<char> &MyQueue){
    if (HT->lchild == NULL && HT->rchild == NULL){
        return HT->code;
    }
    else{
        if (!MyQueue.empty()){
            int n;
            n = MyQueue.front();
            MyQueue.pop();
            if (n == '0'){
                return ChangeCodeToChar(HT->lchild,MyQueue);
            }
            else{
                return ChangeCodeToChar(HT->rchild,MyQueue);
            }
        }
        else{
            return 256;
        }
    }
}

void HuffmanCode(char *FileName){
    ifstream FileIn(FileName,ios::binary);
    char *TempFileName = new char[strlen(FileName) + 1];
    strcpy(TempFileName,FileName);
    strcpy(TempFileName + strlen(TempFileName) - 4,".cod");
    //cout << TempFileName << endl;
    ofstream FileOut(TempFileName,ios::binary | ios::trunc);
    if (FileIn.bad() || FileOut.bad()){
        cout << "文件打开发生错误！" << endl;
        return;
    }
    
    /*
    char ch;
    while ((ch = FileIn.get()) != EOF){
        cout << ch;
    }
    */

    
    StatisticsNode TimeNode[256];
    for (int i = 0;i < 256;i ++){
        TimeNode[i].code = i;
        TimeNode[i].times = 0;
    }
    
    char ch;

    while (!FileIn.eof()){
        ch = FileIn.get();
        TimeNode[(int)((unsigned char)ch)].times ++;
    }
/*
    for (int i = 0;i < 256;i ++){
        cout << (int)(unsigned char)TimeNode[i].code << ":" << TimeNode[i].times << endl;
    }
    system("pause");
*/
    
    sort(TimeNode,TimeNode + 256,CompareTimeNode);
    
/*
    for (int i = 0;i < 256;i ++){
        cout << (int)(unsigned char)TimeNode[i].code << ":" << TimeNode[i].times << endl;
    }
    system("pause");
*/

    
    int n;
    n = 256;
    for (int i = 0;i < 256; i ++){
        if (TimeNode[i].times == 0){
            n = i;
            break;
        }
    }
/*
    for (int i = 0;i < n;i ++){
        cout << (int)(unsigned char)TimeNode[i].code << ":" << TimeNode[i].times << endl;
    }
    system("pause");
*/

   
    HuffmanTree NewHT = CreateHuffmanTree(TimeNode,n);

    //ShowHuffmanTree(NewHT);
    //system("pause");

   
    char *CodeN[256];
    for (int i = 0;i < 256;i ++){
        CodeN[i] = NULL;
    }

    CreateHuffmanCode(CodeN,NewHT,"");
    

    /*
    for (int i = 0;i < 256;i ++){
        cout << i << ":" << (CodeN[i] == NULL ? "NULL" : CodeN[i]) << endl;
    }
    */

    
    FileOut.write("HC",2);
    
    //cout << FileName << endl;
    FileOut.write(FileName,strlen(FileName) + 1);
    
    FileOut.write((char *)&n,sizeof(n));
    
    FileOut.write((char *)TimeNode,sizeof(StatisticsNode) * n);
    
    HANDLE fhadle = CreateFile(FileName,0,0,0,OPEN_EXISTING,0,0);

    long FileLength = GetFileSize(fhadle,0);
    //cout << FileLength << endl;

    FileOut.write((char *)&FileLength,sizeof(FileLength));
    FileIn.clear();
    FileIn.seekg(0);
    queue<char> MyQueue;
    char TempStr[300];
    char TempChar;
    while (!FileIn.eof()){
        ch = FileIn.get();
        
        strcpy(TempStr,CodeN[int((unsigned char)(ch))]);
        //cout << "bin: " << TempStr << endl;
        for (int i = 0;TempStr[i];i ++){
            MyQueue.push(TempStr[i]);
        }
        while (MyQueue.size() >= 8){
            int kk;
            kk = 0;
            for (int i = 7;i >= 0;i --){
                kk += pow(2,i) * (MyQueue.front() - '0');
                MyQueue.pop();
            }
            //cout << "kk:" << kk << endl;
            TempChar = (char)kk;
            FileOut.write(&TempChar,1);
        }
        //CodeN[int(ch)]
    }
 
    int kk = 0,ii = 7;
    while (MyQueue.size() != 0){
        kk += pow(2,ii) * (MyQueue.front() - '0');
        MyQueue.pop();
        ii --;
    }
    TempChar = (char)kk;

    //cout << "kk:" << kk << endl;
    FileOut.write(&TempChar,1);


    FileIn.close();
    FileOut.close();

    HANDLE fhadle2 = CreateFile(TempFileName,0,0,0,OPEN_EXISTING,0,0);
    long FileLength2 = GetFileSize(fhadle2,0);

    delete TempFileName;
    cout << "压缩完毕!压缩比为：" << FileLength2 * 100 / FileLength << "%" << endl;
}

void CreateHuffmanCode(char *Code[],HuffmanTree HT,char *LastCode){
    //cout << "code:" << LastCode << endl;
    if (HT->lchild == NULL && HT->rchild == NULL){
        Code[(int)((unsigned char)(HT->code))] = new char[strlen(LastCode) + 1];
        strcpy(Code[(int)((unsigned char)(HT->code))],LastCode);
    }
    else{
        //cout << "left" << endl;
        char *TempCode = new char[strlen(LastCode) + 2];
        strcpy(TempCode,LastCode);
        CreateHuffmanCode(Code,HT->lchild,strcat(TempCode,"0"));
        //cout << "right" << endl;
        strcpy(TempCode,LastCode);
        CreateHuffmanCode(Code,HT->rchild,strcat(TempCode,"1"));
    }
}

void ShowHuffmanTree(HuffmanTree HT){
    if (HT->lchild == NULL && HT->rchild == NULL){
        cout << (int)(unsigned char)(HT->code) << ":" << HT->weight << endl;
    }
    else{
        cout << "left" << endl;
        ShowHuffmanTree(HT->lchild);
        cout << "right" << endl;
        ShowHuffmanTree(HT->rchild);
    }
}

bool CompareTimeNode(StatisticsNode a,StatisticsNode b){
    return a.times > b.times;
}

bool CompareHTNode(HuffmanTree a,HuffmanTree b){
    return a->weight > b->weight;
}

HuffmanTree CreateHuffmanTree(StatisticsNode *TN,int n){
    if (n <= 1)
        return NULL;

  
    HuffmanTree TreeId[256];

    for(int i = 0;i < n;i ++){
        TreeId[i] = new HTNode;
        TreeId[i]->weight = TN[i].times;
        TreeId[i]->code = TN[i].code;
        TreeId[i]->lchild = NULL;
        TreeId[i]->rchild = NULL;
        TreeId[i]->parent = NULL;
    }

    while (n != 1){
        sort(TreeId,TreeId + n,CompareHTNode);
        HuffmanTree TempHT = new HTNode;
        TempHT->lchild = TreeId[n - 1];
        TempHT->rchild = TreeId[n - 2];
        TempHT->weight = TreeId[n - 1]->weight + TreeId[n - 2]->weight;
        TempHT->parent = NULL;
        TreeId[n - 1]->parent = TempHT;
        TreeId[n - 2]->parent = TempHT;
        TreeId[n - 2] = TempHT;
        n --;
    }
    return TreeId[0];
}
