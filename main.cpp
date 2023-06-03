#include <bits/stdc++.h>
using namespace std;


struct Block {
    unsigned long size;
    struct Block *head;
    struct Block *next;
};

unsigned long *mem = (unsigned long*) malloc(16*100048576);
struct Block *block[11];
vector<Block> num;


void initialize(struct Block** hd) {
        for (int i = 0; i <= 10; i++) {
            block[i] = NULL;
        }
        *hd=(struct Block*) mem;
        struct Block *blo=(struct Block*)mem;//memの先頭アドレスを覚えておく
        
        int size=1024*4096;
        //cout<<"hdの値"<<*hd<<endl;
        block[10]->size=size;
        block[10]->head=blo;
        block[10]->next=blo+size;
        //cout<<block[10]->head<<" "<<block[10]->next<<endl;

        block[10]=block[10]->next;
        block[10]->size=size;
        block[10]->head=blo+size;
        block[10]->next=blo+size*2;
        //cout<<block[10]->head<<" "<<block[10]->next<<endl;

        block[10]=block[10]->next;
        block[10]->size=size;
        block[10]->head=blo+size*2;
        block[10]->next=NULL;
        //cout<<block[10]->head<<" "<<block[10]->next<<endl;

        *hd=blo;//最後にmemの先頭アドレスにheadを戻してあげる

}
int add(struct Block **head,unsigned long *addr,unsigned long ss){
    struct Block *node=*head; /* 今注目しているノードのアドレス */
    struct Block *prev; 
    struct Block *ne=(struct Block*)addr;
    ne->size=ss;
    while (node!=NULL)  {
            if(node>ne) break;
            if(node<ne&&node->next>ne) break;
            prev=node;
            node = node->next;
            
            
    }
    if (node == *head) {
        ne->next = *head; 
        ne->head = *head;
        *head = ne;

    } 
    else {
        prev->next = ne;
        ne->next = node; 
        ne->head = *head;
    }
    

    return 0;


}


int del(struct Block **head,unsigned long *addr){
    struct Block *node; /* 今注目しているノードのアドレス */
    struct Block *prev; 
    node=*head;
    if (*head == NULL) {
        /* リストが空なら何もしない */

        return -1;
    }
    if(node==(struct Block*)addr){
        *head = node->next;
        return 0;
    }

    while (node != NULL) {

        if (node == (struct Block*)addr) {
            /* 削除対象のノードが見つかった */

            /* prevの次のノードをnodeの次のノードに設定 */
            prev->next = node->next;

            /* 削除対象のノードを削除して終了 */

            return 0;
        }

        /* nodeを前のノードとして覚えておく */
        prev = node;

        /* 次のノードを辿る */
        node = node->next;

    }



    /* リストの先頭のノードに注目 */
    
    //free(node);
    return 0;

}



void alloc(unsigned long size,unsigned long motomoto){
    int turn = ceil(log2(size));
    if(block[turn]==NULL){
        alloc(size*2,motomoto);
    }
    else{
        unsigned long *x= (unsigned long*)block[turn];
        del(&block[turn],x);
        while(size!=motomoto){
            turn--;
            size/=2;
            add(&block[turn],x+size*4096,size*4096);
        }
        num.push_back({size,(struct Block*)x,(struct Block*)x+size*4096});
        
    }
}

void fre(struct Block **head,unsigned long *addr,unsigned long ss){
    int turn = ceil(log2(ss/4096));
    add(head,addr,ss);
    struct Block *prev; 
    struct Block *node=*head;
    while (node!=NULL)  {
            prev=node;
            node = node->next;
            //cout<<(unsigned long*)node-(unsigned long*)prev<<endl;
            if(abs((unsigned long*)node-(unsigned long*)prev)==ss){
                del(&block[turn],(unsigned long*)prev);
                del(&block[turn],(unsigned long*)node);
                struct Block *ans=min(prev,node);
                fre(&block[turn+1],(unsigned long*)ans,ss*2);
                //add(&block[turn+1],(unsigned long*)ans,ss*2);
                //cout<<"hello"<<endl;
                break;
            }
    }



}

void print(struct Block *head){
    struct Block *node;
    node = head;
    while (node != NULL) {
        printf("%d:%p  ", node->size/4096, node);
        node = node->next;
    }
    puts("");
}


int main(){
    initialize(&block[10]);
    while(1){
		cout<<"操作　1:ページ確保要求 2:リストの表示 3:終了 4:アロケートリスト:  5:アロケートした領域を戻す";
		int sousa;
		cin>>sousa;
		if(sousa==1){
            int mm; 
            cout<<"何ブロック確保しますか？";
            cin>>mm;
            mm=pow(2,ceil(log2(mm)));
            alloc(mm,mm);
		}
		else if(sousa==2){
            for(int i=10;i>=0;i--){
                printf("%d: ", i);
                print(block[i]);
            }
        }
		
		else if(sousa==3){
			break;
		}
        else if (sousa==4){
            for(auto e:num){
                if(e.size==12345) continue;
                cout<<e.size<<" "<<e.head<<" "<<e.next<<endl;
            }
        }
        else if (sousa==5){
            unsigned long number;
            cout<<"どのブロックを解放しますか？ :"<<endl;
            int ii=0,jj=0;
            if(num.size()==0) continue;
            for(auto e:num){
                if(e.size==12345) {
                    ii++;
                    jj++;
                    continue;
                }
                cout<<ii<<":  "<<e.size<<" "<<e.head<<" "<<e.next<<endl;
                ii++;
                
            }
            if(num.size()-jj<=0) continue;
            cin>>number;
            
            int gg=ceil(log2(num[number].size));
            fre(&block[gg],(unsigned long*)num[number].head,num[number].size*4096);
            num[number].size=12345;
            
        }
		else{
			
			break;
		}
	}
    free(mem);
}
