#include <iostream>
#include <cstdlib> 
#include <ctime>
using namespace std;

int goal[3][3]={{1,2,3},{8,0,4},{7,6,5}};   //Global Goal array
class Node {
    public:
        int h,g,f;
        int puzzle[3][3] = {0};
        Node *neighbor;
        Node *ancestor;
        Node *next;
        int H(){ return h; }
        int G(){ return g; }
        int F(){ return f; }
        Node Neighbor(){ return *neighbor; }
        Node Ancestor(){ return *ancestor; }
        Node Next(){ return *next; }
};
class List{
    public:
    Node* listHead(){   //To create a dummy node
        Node *node = new Node();
        node->h=-9;
        node->g=-9;
        node->f=-9;
        node->next=NULL;
        node->neighbor=NULL;
        node->ancestor=NULL;
        return node;
    }
    int Hn(Node *spot){     //To Calculate the H*(n)
        int h=0;
        for(int x=0; x<3; x++){
            for(int y=0; y<3; y++){
                if(goal[x][y]!=spot->puzzle[x][y]){
                    h++;
                }
            }
        }
        return h;
    }
    void listInsert(Node *Head,Node *newNode){      //insert node in to the list ascending order with f
        Node *spot = new Node();
        spot=findSpot(Head, spot, newNode);
		newNode->next=spot->next;
		spot->next=newNode;
    }
    Node* findSpot(Node *Head,Node *spot,Node *newNode){    //Find spot for smaller F
        spot=Head;
		while((spot->next!=NULL) && ((spot->next->f) <= (newNode->f))){ spot=spot->next; }
		return spot;
    }
    Node * RF(Node *spot){      //To remove the first node on the list
        if(spot->next==NULL){
            return NULL;
        }
        Node * f = spot->next;
        spot->next= spot->next->next;
        f->next=NULL;
        return f; 
    }
    bool isGoal(Node *N){       //To check if the node is the goal puzzle
        for(int x=0; x<3; x++){
            for(int y=0; y<3; y++){
                if(goal[x][y]!=N->puzzle[x][y]){
                    return false;
                }
            }
        }
        return true;
    }
    bool isStart(Node *C,Node *N){      //To check if the two puzzle is the same
        for(int x=0; x<3; x++){
            for(int y=0; y<3; y++){
                if(C->puzzle[x][y]!=N->puzzle[x][y]){
                    return false;
                }
            }
        }
        return true;
    }
    bool isDup(Node *C,Node *N,Node *P){        //To check if ther are any Duplicate
        if(C==NULL){return false;}
        if(C==P){
            if(isStart(C,N)==false){
                return false;
            }else{return true;}
        }else{
            if(isStart(C,N)==false){
                return false;
            }else{return isDup(C->ancestor,N,P);}
        }
        return false;   //Not sure
    }
    void FindNeighbor(Node *C,Node *L,Node *A){     //Find the neighbors
        int r,c;
        for(int x=0; x<3; x++){         //Find the Empty space that is zero
            for(int y=0; y<3; y++){
                if(C->puzzle[x][y]==0){
                    r=x;
                    c=y;
                    break;
                }
            }
        }
        if(r-1>=0){//up
            Node *UpN=new Node();
            for(int x=0; x<3; x++){     //Copy the Puzzle over to the neighbor
                for(int y=0; y<3; y++){
                    UpN->puzzle[x][y]=C->puzzle[x][y];
                }
            }
            UpN->puzzle[r][c]=UpN->puzzle[r-1][c];  //move the piece in the direction
            UpN->puzzle[r-1][c]=0;          //replace moved piece with zero
            if(isDup(C,UpN,A)==false){      //Check if there is a dupe
                UpN->ancestor=C;            //set the ancestor
                listInsert(L,UpN);      //insert to the neighbor list
            }
        }
        if(r+1<3){//down
            Node *DownN=new Node();
            for(int x=0; x<3; x++){     //Copy the Puzzle over to the neighbor
                for(int y=0; y<3; y++){
                    DownN->puzzle[x][y]=C->puzzle[x][y];
                }
            }
            DownN->puzzle[r][c]=DownN->puzzle[r+1][c];      //move the piece in the direction
            DownN->puzzle[r+1][c]=0;                    //replace moved piece with zero
            if(isDup(C,DownN,A)==false){        //Check if there is a dupe
                DownN->ancestor=C;              //set the ancestor
                listInsert(L,DownN);        //insert to the neighbor list
            }
        }
        if(c-1>=0){//left
            Node *LeftN=new Node();
            for(int x=0; x<3; x++){     //Copy the Puzzle over to the neighbor
                for(int y=0; y<3; y++){
                    LeftN->puzzle[x][y]=C->puzzle[x][y];
                }
            }
            LeftN->puzzle[r][c]=LeftN->puzzle[r][c-1];      //move the piece in the direction
            LeftN->puzzle[r][c-1]=0;                    //replace moved piece with zero
            if(isDup(C,LeftN,A)==false){    //Check if there is a dupe
                LeftN->ancestor=C;              //set the ancestor
                listInsert(L,LeftN);       //insert to the neighbor list
            }
        }
        if(c+1<3){//right
            Node *RightN=new Node();
            for(int x=0; x<3; x++){     //Copy the Puzzle over to the neighbor
                for(int y=0; y<3; y++){
                    RightN->puzzle[x][y]=C->puzzle[x][y];
                }
            }
            RightN->puzzle[r][c]=RightN->puzzle[r][c+1];    //move the piece in the direction
            RightN->puzzle[r][c+1]=0;                   //replace moved piece with zero
            if(isDup(C,RightN,A)==false){   //Check if there is a dupe
                RightN->ancestor=C;             //set the ancestor
                listInsert(L,RightN);   //insert to the neighbor list
            }
        }
    }
    bool inListGlow(Node *L,Node *N){       //To order the list for the g value
        Node *spot=new Node();
        spot=L->next;
        while(spot!=NULL){
            int check=0;
            for(int x=0; x<3; x++){
                for(int y=0; y<3; y++){
                    if(spot->puzzle[x][y]==N->puzzle[x][y]){
                        check++;
                    }
                }
            }
            if(check==9){
                if(N->g<spot->g){
                    return true;
                }else{return false;}
            }else{spot=spot->next;}
        }
        return false;
    }
    Node *RM(Node *L,Node *N){  //To remove a given puzzle
        Node *prev=new Node();
        Node *next=new Node();
        prev=L;
        next=prev->next;
        while(next!=NULL){
            int check=0;
            for(int x=0; x<3; x++){
                for(int y=0; y<3; y++){
                    if(next->puzzle[x][y]==N->puzzle[x][y]){
                        check++;
                    }
                }
            }
            if(check==9){
                prev->next=next->next;
                next->next=NULL;
                return next;
            }else{
                next=next->next;
                prev=prev->next;
            }
        }
        return next;
    }
    int HM(Node *spot){     //To calculate the H-Manhattan
        int h;
        for(int x=0;x<3;x++){
            for(int y=0;y<3;y++){
                for(int r=0;r<3;r++){
                    for(int c=0;c<3;c++){
                        if(spot->puzzle[x][y]==goal[r][c] && spot->puzzle[x][y]!=0){
                            h+=abs(x-r)+abs(y-c);
                        }
                    }
                }
            }
        }
        return h;
    }
    void FindDFBBC(Node *C,Node *L,Node *A){    //Find the neighbors
        int r,c;
        for(int x=0; x<3; x++){         //Find the Empty space that is zero
            for(int y=0; y<3; y++){
                if(C->puzzle[x][y]==0){
                    r=x;
                    c=y;
                    break;
                }
            }
        }
        if(r-1>=0){//up
            Node *UpN=new Node();
            for(int x=0; x<3; x++){     //Copy the Puzzle over to the neighbor
                for(int y=0; y<3; y++){
                    UpN->puzzle[x][y]=C->puzzle[x][y];
                }
            }
            UpN->puzzle[r][c]=UpN->puzzle[r-1][c];  //move the piece in the direction
            UpN->puzzle[r-1][c]=0;          //replace moved piece with zero
            UpN->ancestor=C;        //set the ancestor
            FILO(L,UpN);        //insert to the neighbor list
        }
        if(r+1<3){//down
            Node *DownN=new Node();
            for(int x=0; x<3; x++){     //Copy the Puzzle over to the neighbor
                for(int y=0; y<3; y++){
                    DownN->puzzle[x][y]=C->puzzle[x][y];
                }
            }
            DownN->puzzle[r][c]=DownN->puzzle[r+1][c];      //move the piece in the direction
            DownN->puzzle[r+1][c]=0;                    //replace moved piece with zero
            DownN->ancestor=C;          //set the ancestor
            FILO(L,DownN);          //insert to the neighbor list
        }
        if(c-1>=0){//left
            Node *LeftN=new Node();
            for(int x=0; x<3; x++){     //Copy the Puzzle over to the neighbor
                for(int y=0; y<3; y++){
                    LeftN->puzzle[x][y]=C->puzzle[x][y];
                }
            }
            LeftN->puzzle[r][c]=LeftN->puzzle[r][c-1];      //move the piece in the direction
            LeftN->puzzle[r][c-1]=0;                    //replace moved piece with zero
            LeftN->ancestor=C;          //set the ancestor
            FILO(L,LeftN);      //insert to the neighbor list
        }
        if(c+1<3){//right
            Node *RightN=new Node();
            for(int x=0; x<3; x++){     //Copy the Puzzle over to the neighbor
                for(int y=0; y<3; y++){
                    RightN->puzzle[x][y]=C->puzzle[x][y];
                }
            }
            RightN->puzzle[r][c]=RightN->puzzle[r][c+1];        //move the piece in the direction
            RightN->puzzle[r][c+1]=0;                      //replace moved piece with zero
            RightN->ancestor=C;             //set the ancestor
            FILO(L,RightN);       //insert to the neighbor list
        }
    }
    void FILO(Node *Head,Node *newNode){    //Find spot for greater F
        Node *spot = new Node();
        spot=findS(Head, spot, newNode);
		newNode->next=spot->next;
		spot->next=newNode;
    }
    Node* findS(Node *Head,Node *spot,Node *newNode){   //Find spot for greater F
        spot=Head;
		while((spot->next!=NULL) && ((spot->next->f) >= (newNode->f))){ spot=spot->next; }
		return spot;
    }
    void IF(Node *L,Node *N){  //Insert node to the frount of the list
        N->next=L->next;
        L->next=N;
    }

};

int main(int argc,char* argv[]){
    int m;
    cout<< "Please Enter the Search: ";
    cin >> m;
    cout << "Enter 9 numbers: "; // Type a number and press enter
    Node *Spuzzle= new Node();
    for(int x=0; x<3; x++){     //for loop to add the input of the user
        for(int y=0; y<3; y++){
            cin >> Spuzzle->puzzle[x][y]; // Get user input and add to the array
        }
    }
    clock_t t,ans=0,z;   //set a clock
	t = clock();    //t variable clock
    time_t S=time(0);   //Time variable for Start
    time_t End=time(0); //Time variable for Start
    tm *St=localtime(&S);   //Get Local time as the start
    int expanded=0, hr=St->tm_hour, min=St->tm_min; //Keep track of the expanded nodes and the hour, min of the start time
    List *start=new List(); //List variable
    Node *open=new Node();  //Node variable for open list
    Node *close=new Node(); //Node variable for close list
    open=start->listHead(); //Create a dummy node in the open list
    close=start->listHead();    //Create a dummy node in the close list
    Spuzzle->g=0;               //Set the G value
    Spuzzle->h=start->Hn(Spuzzle);  //Set the H value
    Spuzzle->f=Spuzzle->g+Spuzzle->h;   //Set the F value
    start->listInsert(open,Spuzzle);    //Put it in the opoen list
    Node *Cn=new Node();    //Node variable for current puzzle
    if(m==1){
        while(open->next!=NULL && start->isGoal(Cn)==false){    //While loop for checking if there is a solution 
            Cn=start->RF(open); //Current puzzle from the openlist
            tm *TN=localtime(&End); //Get current time
            if(TN->tm_hour==hr+1){//Check if the program has run for 30min
                int O=0;
                O=60-min;
                O+=TN->tm_min;  //if the start time of program was < 30 min
                if(O>=30){
                    cout << "The Program has ran for 30 min with out an answer" << endl;
                    return 0;
                }
            }else if(min+30==TN->tm_min){   //if the min adds up to 30 min
                cout << "The Program has ran for 30 min with out an answer" << endl;
                return 0;
            }
            if(start->isGoal(Cn)==true){    //check if the puzzle is the goal puzzle
                t=clock()-t;    //get the total time of the program
                cout << "Program took " << (float)t/CLOCKS_PER_SEC << " Seconds"<< endl;
                cout << "Number of Nodes Expanded: " << expanded <<endl;
                Node *sol=new Node();
                sol=Cn;
                while(sol!=NULL){               //Print the optimal solution from goal to start
                    for(int x=0; x<3; x++){         
                        for(int y=0; y<3; y++){
                            cout << sol->puzzle[x][y];
                        }
                    cout <<endl;
                    }
                    cout <<endl;
                    sol=sol->ancestor;
                }
                return 0;   //exit progrm
            }
            else{   //If solution not found
                Node *Cnnei=new Node();
                Cnnei=start->listHead();
                expanded++;
                start->FindNeighbor(Cn,Cnnei,Spuzzle);  //Expand the current puzzle
                while(Cnnei->next!=NULL){   //While there are neighbors 
                    Node *Neig=new Node();
                    Neig=start->RF(Cnnei);      //Set Neig to a neighbor
                    Neig->g=Neig->ancestor->g+1;    //Set G value
                    Neig->h=start->Hn(Neig);        //Set H value
                    Neig->f=Neig->g+Neig->h;        //Set F value
                    if(start->inListGlow(close,Neig)==true){//in closed list and has lower g value
                        Node *temp=new Node();
                        temp=start->RM(close,Neig);     //Remove puzzle from close list
                        start->listInsert(open,temp);   //Add into the open list
                        temp->ancestor=Cn;              //Set its ancestor
                    }else if(start->inListGlow(open,Neig)==true){//in open list and has lower g value
                        Node *temp2=new Node();
                        temp2=start->RM(open,Neig);         //Remove puzzle from open list
                        start->listInsert(open,temp2);      //Add into the open list
                        Neig->ancestor=Cn;      //Set its ancestor
                    }else{//in niether open and close
                        start->listInsert(open,Neig);
                        Neig->ancestor=Cn;      //Set its ancestor
                    }
                }
            }
        }
    }else if(m==2){
        while(open->next!=NULL && start->isGoal(Cn)==false){    //While loop for checking if there is a solution 
            Cn=start->RF(open); //Current puzzle from the openlist
            tm *TN=localtime(&End); //Get current time
            if(TN->tm_hour==hr+1){//Check if the program has run for 30min
                int O=0;
                O=60-min;
                O+=TN->tm_min;  //if the start time of program was < 30 min
                if(O>=30){
                    cout << "The Program has ran for 30 min with out an answer" << endl;
                    return 0;
                }
            }else if(min+30==TN->tm_min){   //if the min adds up to 30 min
                cout << "The Program has ran for 30 min with out an answer" << endl;
                return 0;
            }
            if(start->isGoal(Cn)==true){    //check if the puzzle is the goal puzzle
                t=clock()-t;    //get the total time of the program
                cout << "Program took " << (float)t/CLOCKS_PER_SEC << " Seconds"<< endl;
                cout << "Number of Nodes Expanded: " << expanded <<endl;
                Node *sol=new Node();
                sol=Cn;
                while(sol!=NULL){               //Print the optimal solution from goal to start
                    for(int x=0; x<3; x++){         
                        for(int y=0; y<3; y++){
                            cout << sol->puzzle[x][y];
                        }
                    cout <<endl;
                    }
                    cout <<endl;
                    sol=sol->ancestor;
                }
                return 0;   //exit progrm
            }
            else{   //If solution not found
                Node *Cnnei=new Node();
                Cnnei=start->listHead();
                expanded++;
                start->FindNeighbor(Cn,Cnnei,Spuzzle);  //Expand the current puzzle
                while(Cnnei->next!=NULL){   //While there are neighbors 
                    Node *Neig=new Node();
                    Neig=start->RF(Cnnei);      //Set Neig to a neighbor
                    Neig->g=Neig->ancestor->g+1;    //Set G value
                    Neig->h=start->HM(Neig);        //Set H value
                    Neig->f=Neig->g+Neig->h;        //Set F value
                    if(start->inListGlow(close,Neig)==true){//in closed list and has lower g value
                        Node *temp=new Node();
                        temp=start->RM(close,Neig);     //Remove puzzle from close list
                        start->listInsert(open,temp);   //Add into the open list
                        temp->ancestor=Cn;              //Set its ancestor
                    }else if(start->inListGlow(open,Neig)==true){//in open list and has lower g value
                        Node *temp2=new Node();
                        temp2=start->RM(open,Neig);         //Remove puzzle from open list
                        start->listInsert(open,temp2);      //Add into the open list
                        Neig->ancestor=Cn;      //Set its ancestor
                    }else{//in niether open and close
                        start->listInsert(open,Neig);
                        Neig->ancestor=Cn;      //Set its ancestor
                    }
                }
            }
        }
    }else if(m==3){
        int L=Spuzzle->f;   //Set L value to close to infinite
        while(start->isGoal(Cn)==false){
            Node *BN=new Node();
            BN=start->listHead();
            while(open->next!=NULL){
                Cn=start->RF(open); //Current puzzle from the openlist
                tm *TN=localtime(&End); //Get current time
                if(TN->tm_hour==hr+1){//Check if the program has run for 30min
                    int O=0;
                    O=60-min;
                    O+=TN->tm_min;  //if the start time of program was < 30 min
                    if(O>=30){
                        cout << "The Program has ran for 30 min with out an answer" << endl;
                        return 0;
                    }
                }else if(min+30==TN->tm_min){   //if the min adds up to 30 min
                    cout << "The Program has ran for 30 min with out an answer" << endl;
                    return 0;
                }
                if(start->isGoal(Cn)==true){    //if current node is the goal puzzle compute f
                    t=clock()-t;    //get the total time of the program
                    cout << "Program took " << (float)t/CLOCKS_PER_SEC << " Seconds"<< endl;
                    cout << "Number of Nodes Expanded: " << expanded <<endl;
                    Node *sol=new Node();
                    sol=Cn;
                    while(sol!=NULL){               //Print the optimal solution from goal to start
                        for(int x=0; x<3; x++){         
                            for(int y=0; y<3; y++){
                                cout << sol->puzzle[x][y];
                            }
                        cout <<endl;
                        }
                        cout <<endl;
                        sol=sol->ancestor;
                    }
                    return 0;   //exit progrm
                }else{
                    Node *Cnnei=new Node();
                    Cnnei=start->listHead();        //list of neighbors
                    expanded++;
                    start->FindDFBBC(Cn,Cnnei,Spuzzle);  //Expand the current puzzle
                    while(Cnnei->next!=NULL){       //While there are neighbors
                        Node *Neig=new Node();
                        Neig=start->RF(Cnnei);      //Set Neig to a neighbor
                        Neig->g=Neig->ancestor->g+1;    //Set G value
                        Neig->h=start->HM(Neig);        //Set H value
                        Neig->f=Neig->g+Neig->h;        //Set F value
                        if(Neig->f<L){              //If the f value is less than L
                            start->IF(open,Neig); //Put the neighbor in to the open list
                        }else{
                            start->listInsert(BN,Neig); //Add the neighbors that have bigger value to list in ascending order
                        }
                    }
                }
            }
        Spuzzle->next=NULL;
        start->listInsert(open,Spuzzle);    //Put it in the open list
        L=start->RF(BN)->f;
        }
    }else if(m==4){
        int L=99999;   //Set L value to close to infinite
        while(open->next!=NULL){
            Cn=start->RF(open); //Current puzzle from the openlist
            tm *TN=localtime(&End); //Get current time
            if(TN->tm_hour==hr+1){//Check if the program has run for 30min
                int O=0;
                O=60-min;
                O+=TN->tm_min;  //if the start time of program was < 30 min
                if(O>=30){
                    cout << "The Program has ran for 30 min with out an answer" << endl;
                    return 0;
                }
            }else if(min+30==TN->tm_min){   //if the min adds up to 30 min
                cout << "The Program has ran for 30 min with out an answer" << endl;
                return 0;
            }
            if(start->isGoal(Cn)==true){    //if current node is the goal puzzle compute f
                if(Cn->f<L){                //If the f value is less than the L then update L
                    L=Cn->f;
                }
                z=clock()-t;        //Current solution time - start time
                ans=z-ans;          //Optimal solution time=current solution - previous solution time
            }else{
                Node *Cnnei=new Node();
                Cnnei=start->listHead();        //list of neighbors
                expanded++;
                start->FindDFBBC(Cn,Cnnei,Spuzzle);  //Expand the current puzzle
                while(Cnnei->next!=NULL){       //While there are neighbors
                    Node *Neig=new Node();
                    Neig=start->RF(Cnnei);      //Set Neig to a neighbor
                    Neig->g=Neig->ancestor->g+1;    //Set G value
                    Neig->h=start->HM(Neig);        //Set H value
                    Neig->f=Neig->g+Neig->h;        //Set F value
                    if(Neig->f<L){              //If the f value is less than L
                        start->IF(open,Neig); //Put the neighbor in to the open list
                    }
                }
            }
        }
        t=clock()-t;    //get the total time of the program
        cout << "Program took " << (float)t/CLOCKS_PER_SEC << " Seconds."<< endl;
        cout << "Optimal solution took " << (float)ans/CLOCKS_PER_SEC << " Seconds."<< endl;
        cout << "Number of Nodes Expanded: " << expanded <<endl;
        Node *sol=new Node();
        sol=Cn;
        while(sol!=NULL){               //Print the optimal solution from goal to start
            for(int x=0; x<3; x++){          
                for(int y=0; y<3; y++){
                    cout << sol->puzzle[x][y];
                }
            cout <<endl;
            }
            cout <<endl;
            sol=sol->ancestor;
        }
        return 0;   //exit progrm
    }else{
        cout << "Error Wrong Search " <<endl;
    }
}
