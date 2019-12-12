#include <iostream>
#include <fstream>
#include <cstring>
#include <stdlib.h>
#include <vector>
using namespace std;

class Staff{
    protected://����������Ҳ�ܼ̳�
    string no; //���
    string name; //����
    int age; //����

    public:
    Staff(string no, string name, int age){
        this->no = no;
        this->name = name;
        this->age = age;
    }
    virtual ~Staff(){}
    //����Ϊpublic, ����ӿ�
    virtual void showData() const = 0; //���Ա����Ϣ(���麯��-���������ʵ��)
    virtual string getTitle() const = 0;//�õ�Ա����ְ��(���麯��)
    string getNo() const{ return no;} //�õ�Ա���ı��(������)
    void setNo(string no){this->no = no;};//�޸�Ա�����
    string getName() const{return name;}//�õ�Ա��������
    void setName(string name){this->name = name;};//�޸�Ա������
    int getAge() const{return age;}//�õ�Ա��������
    void setAge(int age){this->age = age;};//�޸�Ա������
    virtual double getSales()const{}//�õ�Ա�����۶�(����еĻ�)(�麯��-������ɲ�ʵ��)
    virtual void setSales(double sales){}//�޸�Ա�����۶�(����еĻ�) 
};

class Salesman: virtual public Staff{
    protected:
    double sales;//���۶�

    public:
    Salesman(string no, string name, int age, double sales): Staff(no, name, age){
        this->no = no;
        this->name = name;
        this->age = age;
        this->sales = sales;
    }
    virtual ~Salesman(){}

    virtual void showData() const{ //���麯����д��ʽ���ϸ� 
        cout<<"���:"<<no<<" ";
        cout<<"����:"<<name<<" ";
        cout<<"����:"<<age<<" ";
        cout<<"ְ��:"<<"Salesman"<<"     ";
        cout<<"���۶�:"<<sales<<endl;
    }

    virtual string getTitle() const{//��д,���ظ�����ְ��
        return "Salesman";
    }

    double getSales() const{return this->sales;}
    void setSales(double sales){this->sales = sales;};//�޸�Ա��Sales
};


class Manager: virtual public Staff{
    //���ݳ�Ա���ɻ���̳���
    public:
    Manager(string no, string name, int age): Staff(no, name, age){
        this->no = no;
        this->name = name;
        this->age = age;
    }
    virtual ~Manager(){}

    virtual void showData() const{
        cout<<"���:"<<no<<" ";
        cout<<"����:"<<name<<" ";
        cout<<"����:"<<age<<" ";
        cout<<"ְ��:"<<"Manager"<<endl;
    }
    virtual string getTitle() const{
        return "Manager";
    }
    //��Ȼ�̳���getSales()��setSales()��������Ϊ��
};

class SalesManager: public Salesman, public Manager{
    //sales��Salesman�̳���
    public:
    SalesManager(string no, string name, int age, double sales): Staff(no, name, age),
    Salesman(no, name, age, sales), Manager(no, name, age){
        this->no = no;
        this->name = name;
        this->age = age;
        this->sales = sales;
    }
    virtual ~SalesManager(){}

    virtual void showData() const{
        cout<<"���:"<<no<<" ";
        cout<<"����:"<<name<<" ";
        cout<<"����:"<<age<<" ";
        cout<<"ְ��:"<<"SalesManager"<<" ";
        cout<<"���۶�:"<<sales<<endl;
    }
    virtual string getTitle() const{
        return "SalesManager";
    }
    double getSales() const{return this->sales;}//��ֹ����, ��д
    void setSales(double sales){this->sales = sales;};
};

struct Node{ //vector�е�Ԫ��
    Staff *staff;//ָ��Staff��ָ��
    bool isEmployed;//�������
};

class ManagementSystem{
    private:
    vector<Node> vec;//��̬����
    int StaffNum;//Ա��������
    //private,�����ܷ�װ, �û�ֻ��ͨ��SelectFunction()�ӿڵ���
    void fileIn();//�ļ�¼��
    void showStaff();//��ʾ������Ϣ
    void showCount();//��ʾ����Ա������
    void searchStaff();//��ʾ�����˵���Ϣ
    void addData();//���Ա��
    void markStaffToBeFired();//��Ǵ����Ա��
    void updateStaff();//�޸�Ա����Ϣ
    void fireStaffs();//���isEmployedΪfalse��Ա��
    void recombineFile();//�����ļ����˳�

    //��������
    void hire(string no, string name, int age, string title, double sales = 0);//��vector���¼ӽ��
    bool hasRepeatted(string no);//Ա����Ϊ��,�ж�vector���Ƿ��Ѵ�����ͬԱ����
    bool inTitles(string title){//�ж����title�Ƿ���ȷ
        return (!title.compare("Salesman"))||(!title.compare("Manager"))||(!title.compare("SalesManager"));
    }
    bool hasSales(string title){//�ж��Ƿ�ӵ��sales������ݳ�Ա
        return ((!title.compare("Salesman")) || (!title.compare("SalesManager")));}
    bool isSalesman(string title){return (!title.compare("Salesman"));}//�ж��Ƿ���Manager
    bool isManager(string title){return (!title.compare("Manager"));}
    bool isSalesManager(string title){return (!title.compare("SalesManager"));}
    int getStaffNum(){return StaffNum;}

    public:
    ManagementSystem();
    ~ManagementSystem(){}
    void SelectFunction();//ѡ����
};

ManagementSystem::ManagementSystem(){
    this->StaffNum = 0;
    fileIn();//���ӿ�
}

void ManagementSystem::fileIn(){
    string no, name, title;
    int age;
    double sales;

    ifstream infile("staff.txt", ios::in);
    if(!infile) cout<<"�ļ��´���, �������Ա���ٲ���!\n"<<endl;
    infile.seekg(0, ios::end); //���ļ�ָ��ָ���ļ�ĩ�� 
	streampos fp = infile.tellg(); //fpΪ�ļ�ָ���ƫ���� 
	if (int(fp) == 0) cout<<"�ļ�Ϊ��, �������Ա���ٲ���!\n"<<endl;
	infile.seekg(0, ios::beg); //���ļ�ָ��ָ���ļ�ĩ��
    while(infile >> no >> name >> age >> title){ //����¼������
        sales = 0;
        if (hasSales(title)) infile >> sales;
        hire(no, name, age, title, sales);
    }
}

//������ĩβ���һ���½��, ͬʱStaffNum++;
void ManagementSystem::hire(string no, string name, int age, string title, double sales){
    Node *newNode = (Node *) malloc(sizeof(Node)); //����һ���½��Ŀռ�, ���� newNodeָ����
    //���ݲ�ͬ��ְλ,���ɲ�ͬ��Node
    if (isSalesman(title)) {
        newNode->staff = new Salesman(no, name, age, sales);;
        newNode->isEmployed = true;
    } else if (isManager(title)) {
        newNode->staff = new Manager(no, name, age);
        newNode->isEmployed = true;
    } else if (isSalesManager(title)) {
        newNode->staff = new SalesManager(no, name, age, sales);
        newNode->isEmployed = true;
    }
    else{
        cout<<"�������, ����������:"<<endl;
    }
    vec.push_back(*newNode);
    StaffNum++;//Ա����Ҫ�ǵü�1
}

//�������Ա������Ϣ
void ManagementSystem::showStaff(){
    for(Node it: vec){
            cout << "isEmployed:" << it.isEmployed <<" ";
            it.staff->showData(); 
    }
}

//�ж�Ա�����Ƿ��ظ�
bool ManagementSystem::hasRepeatted(string no){
    vector<Node>::iterator it;
        for(it = vec.begin(); it != vec.end(); it++){
            if(!no.compare(it->staff->getNo())){
                return true;
            }     
        }
}

//��Ա���Ż�������������Ϣ
void ManagementSystem::searchStaff(){
	string searchNo, searchName;
    while(1){
    int c;
    cout<<"ѡ��ؼ���(��0�˳�):(1)����Ա����(2)��������"<<endl;
    cin>>c;
    if (cin.good() == 1);//�ж��Ƿ������������
    else{cout<<"�������."<<endl;cin.clear();cin.sync();continue;}
    if(c == 1){
        cout<<"������Ա����:";
        cin>>searchNo;
        for(Node it: vec){
           if(!searchNo.compare(it.staff->getNo())){
                cout << "isEmployed:" << it.isEmployed <<" ";
                it.staff->showData();
                return;
            }      
        }
        cin.ignore(100, '\n');//��ֹ���������Ϣ
        cout<<"���޴���."<<endl;        
        break;
    }
    else if(c == 2){
    	bool flag = false; 
        cout<<"������Ա������:";//�迼�ǿ������������
        cin>>searchName;
        for(Node it: vec){
           if(!searchName.compare(it.staff->getName())){
                cout << "isEmployed:" << it.isEmployed <<" ";
                it.staff->showData();
                flag = true;
            }   
        }
        if(flag) return;
        cin.ignore(100, '\n');
        cout<<"���޴���."<<endl;
        break;
    }
    else if(c == 0) return;
    else{
        cout<<"�������, ����������"<<endl;
    }
    }
}

//�������
void ManagementSystem::showCount(){
    int SalesmanNum=0, ManagerNum=0, SalesmanagerNum=0;
    vector<Node>::iterator it;
    for(it = vec.begin(); it != vec.end(); it++){
        string ss = it->staff->getTitle(); 
        if(isSalesman(ss)) SalesmanNum++;
        else if(isManager(ss)) ManagerNum++;
        else if(isSalesManager(ss)) SalesmanagerNum++;
        else{}
    }
    cout<<"������:"<<getStaffNum()<<endl<<"Salesman����:"<<SalesmanNum
        <<endl<<"Manager����:"<<ManagerNum<<endl<<"Salesmanager����:"<<SalesmanagerNum;
}

void ManagementSystem::addData(){//�迼��Ա�����ظ����
    int age;
    string no, name, title;
    double sales;
    while(1){
    cout<<"������Ա���ţ����������䣬ְλ��(Ӫ����):(������˳�������'#'"<<endl;
    cin>>no; //�ж������Ƿ�Ϊ#
    if(!no.compare("#")) return;
    cin>>name;
    cin>>age;//age ��Ϊint��
    if (cin.good() == 1){cin.ignore(100, ' ');cin>>title;}//�����double����ֻȡС����ǰ�� 
    else{cout<<"������Ϊ����."<<endl;cin.clear();cin.sync();continue;}
    //cin.ignore(100, ' ');//�����double����ֻȡС����ǰ�� 
    if(inTitles(title));// titleֻ��Ϊ3��
    else{cout<<"ְλ�������."<<endl;cin.ignore(100, '\n');continue;}
    if(hasRepeatted(no)){cout<<"Ա������ظ�."<<endl;continue;}
    if (hasSales(title)){
    	while(1){
    		cin >> sales;cin.ignore(100, '\n');//��ֹ�û�����������
			if (cin.good() == 1);
    		else{cout<<"���۶���Ϊ����."<<endl;cin.clear();cin.sync();continue;}
    		break; 
		}
	}
    else{
    	if(cin.eof()){//��ֹ�û��������� 
    		cout<<"(��Ա�������������۶�."<<endl; 
    		cin.ignore(100, '\n');	
		}
	} 
	
    hire(no, name, age, title, sales);//¼�벢�˳�����
    break;
    }
    cout << "Ա����Ϣ��¼��" << endl;
}

void ManagementSystem::markStaffToBeFired(){
	string searchNo, searchName;
    while(1){
    cout<<"ѡ��ؼ���(��0�˳�):(1)����Ա����(2)��������"<<endl;
    int c;cin>>c;
    if (cin.good() == 1);
    else{cout<<"�������."<<endl;cin.clear();cin.sync();continue;}
    if(c == 1){
        cout<<"������Ա����:";
        cin>>searchNo;
        for(Node &it: vec){//�˴���������
           if(!searchNo.compare(it.staff->getNo())){
                it.isEmployed = false;
                cout<<"�ѱ��"<<endl; 
                return;
            }      
        }
        cin.ignore(100, '\n');
        cout<<"���޴���"<<endl;
        break;
    }
    else if(c == 2){
        cout<<"������Ա������:";//�迼�ǿ������������
        cin>>searchName;
        for(Node &it: vec){
           if(!searchName.compare(it.staff->getName())){
                it.isEmployed = false;
                cout<<"�ѱ��"<<endl; 
                return; 
            }     
        }
        cin.ignore(100, '\n');
        cout<<"���޴���"<<endl;
        break;
    }
    else if(c == 0){return;}
    else{
        cout<<"�������, ����������"<<endl;
    }
    }
}

void ManagementSystem::updateStaff(){
    int c, age;
    string searchNo, no, name, title;
    double sales;
    cout<<"������Ա����:";
    cin>>searchNo;

    vector<Node>::iterator it;
    for(it = vec.begin(); it != vec.end();){
        if(!searchNo.compare(it->staff->getNo())){
            no = it->staff->getNo();
            name = it->staff->getName();
            age = it->staff->getAge();
            while(1){           
            	while(1){
                	cout<<"1.Ա����2.����3.����4.ְλ(5.Ӫ����)\nҪ�޸�(�˳�������0):";
                	cin>>c;
                	if (cin.good() == 1);//������Ϊ����
                	else{cout<<"�������."<<endl;cin.clear();cin.sync();continue;}
                	break;
            	}
            
            if(c == 0) return;//��0���˳� 
            switch (c)
            {
            case 1:
                cout<<"������Ա����:";cin>>no;
                cin.ignore(100, '\n');
                it->staff->setNo(no);
                break;
            case 2:
                cout<<"������Ա������:";cin>>name;
                cin.ignore(100, '\n');
                it->staff->setName(name);
                break;
            case 3:
                while(1){
                    cout<<"������Ա������:";cin>>age;
                    if (cin.good() == 1) cin.ignore(100, ' ');
                    else{cout<<"������Ϊ����."<<endl;cin.clear();cin.sync();continue;}
                    break;
                }
                it->staff->setAge(age);
                break;
            case 4:
                while(1){
                cout<<"������Ա��ְλ:";cin>>title;
                cin.ignore(100, '\n');
                if(inTitles(title)) ;// titleֻ��Ϊ3��
                else{cout<<"ְλ�������."<<endl;cin.ignore(100, '\n');continue;}
                break;
                }

                if(isManager(it->staff->getTitle())){
                    if(isManager(title)){cout<<"ְλ�ظ�"<<endl;return;}
                    else{
                        cout<<"�������������۶�:";
                        cin>>sales;
                        Node *newNode = (Node *) malloc(sizeof(Node)); //����һ���½��Ŀռ�, ����newNodeָ����
                        if (isSalesman(title)) {
                            newNode->staff = new Salesman(no, name, age, sales);
                            newNode->isEmployed = true;
                        }
                        else{
                            newNode->staff = new SalesManager(no, name, age, sales);
                            newNode->isEmployed = true;
                        }
                        
                        it = vec.erase(it); //����ָ��ɾ��Ԫ�ص���һ����ָ�� 
                        it = vec.insert(it, *newNode);//��ָ��Ԫ��ǰ����һ��Ԫ�� 
                    }
                }
                else{
                    if(!title.compare(it->staff->getTitle())){
                        cout<<"ְλ�ظ�"<<endl;
                        return;
                    }
                    else{
                        if(isManager(title)){
                            Node *newNode = (Node *) malloc(sizeof(Node)); 
                            newNode->staff = new Manager(no, name, age);
                            newNode->isEmployed = true;
                            it = vec.erase(it); //����ָ��ɾ��Ԫ�ص���һ����ָ�� 
                            it = vec.insert(it, *newNode);//��ָ��Ԫ��ǰ����һ��Ԫ�� 
                        }
                        else if(isSalesman(title)){
                            Node *newNode = (Node *) malloc(sizeof(Node)); 
                            newNode->staff = new Salesman(no, name, age, it->staff->getSales());
                            newNode->isEmployed = true;
                            it = vec.erase(it); 
                            it = vec.insert(it, *newNode);
                        }
                        else{
                            Node *newNode = (Node *) malloc(sizeof(Node));
                            newNode->staff = new SalesManager(no, name, age, it->staff->getSales());
                            newNode->isEmployed = true;
                            it = vec.erase(it); 
                            it = vec.insert(it, *newNode);
                        }
                    }

                }
                break;
            case 5:
                if(isManager(it->staff->getTitle())){
                    cout<<"���������۶�һ˵."<<endl;
                    return;
                }
                else{
                    cout<<"���������۶�:";cin>>sales;
                    cin.ignore(100, '\n');
                    it->staff->setSales(sales);
                }
                break;
            
            default:
                break;
            }
			cout << "Ա����Ϣ���޸�." << endl;
            return;
            }      
        }
        else it++; 
    
    }
    cout<<"���޴���"<<endl;
}

void ManagementSystem::fireStaffs(){
    vector<Node>::iterator it;
    for(it = vec.begin(); it != vec.end();){
        if(it->isEmployed == false){
            it = vec.erase(it); //����ָ��ɾ��Ԫ�ص���һ����ָ��
        }//���ɾ���������һ��Ԫ����ָ��ɾ��������һ�� 
        else it++;
    }
    cout<<"��ɲ���"<<endl;
}

void ManagementSystem::recombineFile(){
    fireStaffs();//����ǰɾ�������Ա��
    ofstream outfile("staff.txt", ios::out | ios::trunc);//��д�ļ�
    vector<Node>::iterator it;
    for(it = vec.begin(); it != vec.end(); it++){
        outfile<<it->staff->getNo()<<" "<<it->staff->getName()
		<<" "<<it->staff->getAge()<<" "<<it->staff->getTitle()<<" ";
        if(!isManager(it->staff->getTitle()))
	    	outfile<<it->staff->getSales()<<endl;
        else outfile<<endl; 
    }
    outfile.close();
}

void ManagementSystem::SelectFunction(){
    int c, age;
    string no, name, title;
    double sales;
    cout<<"��ӭʹ�����۹�˾Ա������ϵͳ!"<<endl;
    while(true){
    	cout<<
		" -------------------------------------------------\n";
        cout<<
		"    ѡ����:(1)��ѯ����Ա����Ϣ (2)��ѯԱ����Ϣ\n"<<
		" (3)��ѯ���� (4)��Ǵ����Ա��   (5)����ѱ��Ա��\n"<<
		" (6)���Ա�� (7)�޸�Ա����Ϣ     (0)���沢�˳�    \n"<<
		" -------------------------------------------------"<<endl;
		cout<<"ѡ��:"; 
        cin>>c;
        if (cin.good() == 1);
        else{cout<<"�������, ����������."<<endl;cin.clear();cin.sync();continue;}
        switch (c)
        {
        case 1:
            showStaff();cout<<endl;
            break;
        case 2:
            searchStaff();cout<<endl;
            break;
        case 3:
            showCount();cout<<endl;
            break;
        case 4:
            markStaffToBeFired();
            break;
        case 5:
            fireStaffs();cout<<endl;
            break;
        case 6:
            addData();cout<<endl;
            break;
        case 7:
            updateStaff();cout<<endl;
            break;
        case 0:
            recombineFile();cout<<endl;
            break;
        default:
            cout<<"�������, ����������."<<endl;
            cin.ignore(100, '\n');//��ֹ�������� ��������� 
            break;
        }
        if(c == 0) break;
    }
}

int main(){
    ManagementSystem ms;
    ms.SelectFunction();
    return 0;
}
