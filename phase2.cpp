#include <bits/stdc++.h>
using namespace std;
// char M[300][4];
vector<string> M(300);
vector<char> R(4);
vector<char> IR(4);
vector<int> IC(2);
bool C;
int PTR, PTE;
int Job_ID, TTL, TLL, TTC, TLC;
int RA, VA;
int SI = 0, PI = 0, TI = 0;
unordered_set<int> un;
ifstream input_file;
int ALLOCATE();
void loadToMemory(string buffer);
void memory_data();
void startExecution();
void executeUserProgram();
void decrementIC();
int ADDRESS_MAP(int n);
void terminate(int n);
void read();
void write();
void MOS();
void init();

int main(int argc, char const *argv[])
{
    input_file.open("input.txt");
    string buffer = "";
    while (!input_file.eof())
    {
        getline(input_file, buffer);
        string temp = buffer.substr(0, 4);

        if (temp == "$AMJ")
        {
            init();
            Job_ID = stoi(buffer.substr(4, 4));
            TTL = stoi(buffer.substr(8, 4));
            TLL = stoi(buffer.substr(12, 4));
            PTR = ALLOCATE() * 10;
            cout << "PTR :" << PTR << "\n";
        }
        else if (temp == "$DTA")
        {
            startExecution();
        }
        else if (temp == "$END")
        {
            cout << "Program ended successfully\n";
        }
        else if (temp != "$AMJ" && temp != "$DTA" && temp != "$END")
        {
            loadToMemory(buffer);
            // memory_data();
            // exit(0);
        }
    }

    return 0;
}

void init()
{
    M.clear();
    R.clear();
    IR.clear();
    IC.clear();
    M.resize(100, " ");
    R.resize(4, ' ');
    IR.resize(4, ' ');
    IC.resize(2, -1);
    C = false;
    SI = 3;
    Job_ID = 0;
    TTL = 0;
    TLL = 0;
    TTC = 0;
    TLC = 0;
    RA = 0;
    VA = 0;
}

int ALLOCATE()
{
    int min = 0;
    int range = 30;
    int num = rand() % range + min;

    if (M[num * 10] == "")
    {
        return num;
    }
    else
    {
        ALLOCATE();
    }
}

void loadToMemory(string buffer)
{
    // getting frame for program
    int frame = ALLOCATE();
    // updating page table
    M[PTR] = to_string(frame);
    // storing program
    int n = frame * 10;
    int N = buffer.length();
    int trav = (N / 4) + 1;
    int i = 0;
    while (trav--)
    {
        string temp = buffer.substr(i, 4);
        i += 4;
        M[n] = temp;
        n++;
    }
}
void read()
{
    string buffer2 = "";
    getline(input_file, buffer2);
    if (buffer2.substr(0, 4) == "$END")
    {
        // end prog
        // terminate 1
        terminate(1);
        return;
    }
    else
    {
        IR[3] = '0';
        int memory = ((IR[2] - '0') * 10) + (IR[3] - '0');
        int m = ADDRESS_MAP(memory);
        int N = buffer2.length();
        int trav = (N / 4) + 1;
        int i = 0;
        while (trav--)
        {
            string temp = buffer2.substr(i, 4);
            i += 4;
            M[m] = temp;
            m++;
        }
    }
    TTC++;
    TLC++;
    // memory_data();
    // exit(0);
}

void write()
{

    TLC++;
    TTC++;
    if (TLC > TTL)
    {
        // terminate 2
        // line limit exceeded
        terminate(2);
        exit(0);
    }
    else
    {
        IR[3] = '0';
        fstream output_file;
        output_file.open("output.txt", ios::app);
        int memory = ((IR[2] - '0') * 10) + (IR[3] - '0');
        int m = ADDRESS_MAP(memory);

        for (int i = m; i < m + 10; i++)
        {
            output_file << M[i];
        }
        output_file << "\n";
    }
}
void terminate(int n)
{
    fstream output_file;
    output_file.open("output.txt", ios::app);
    output_file << "\n";
    output_file << "\n";
    switch (n)
    {
    case 0:
        output_file << "No Error\n";
        break;
    case 1:
        output_file << "Out of Data\n";
        break;
    case 2:
        output_file << "Line Limit Exceeded\n";
        break;
    case 3:
        output_file << "Time Limit Exceeded\n";
        break;
    case 4:
        output_file << "Operation Code Error\n";
        break;
    case 5:
        output_file << "Operand Error\n";
        break;
    case 6:
        output_file << "Invalid Page Fault\n";
        break;
    default:
        break;
    }
    return;
}
void MOS()
{
    // cout << "Inside Moss";
    if (TI == 0)
    {
        if (SI == 1)
        {
            // read
            read();
            SI = 0;
            return;
        }
        if (SI == 2)
        {
            // write
            write();
            SI = 0;
            return;
        }
        if (SI == 3)
        {
            // terminate 0
            terminate(0);
            return;
        }
        if (PI == 1)
        {
            // terminate 4
            // operation code error
            terminate(4);
            PI = 0;
            // return;
            return;
        }
        if (PI == 2)
        {
            terminate(5);
            PI = 2;
            return;
            // exit(0);
        }
        if (PI == 3)
        {
            // page fault valid then adjust
            // invalid page fault then terminate 6
            if (IR[0] == 'G' && IR[1] == 'D')
            {
                cout << "\nValid Page Fault\n";
                int frame = ALLOCATE();
                cout << frame << "\n";
                // updating page table
                M[PTE] = to_string(frame);
                TTC++;
                decrementIC();
                executeUserProgram();
            }
            else if (IR[0] == 'S' && IR[1] == 'R')
            {
                cout << "\nValid Page Fault\n";
                int frame = ALLOCATE();
                cout << frame << "\n";
                // updating page table
                M[PTE] = to_string(frame);
                TTC++;
                decrementIC();
                executeUserProgram();
            }
            else
            {
                // invalid page fault
                cout << "Invalid page fault\n";
                terminate(6);
                return;
            }
            PI = 0;
        }
    }
}
int ADDRESS_MAP(int n)
{
    // returns RA or sets Pi
    if (n > 99 || n < 0)
    {
        // operand error
        PI = 2;
        MOS();
        // return;
    }
    else
    {
        stringstream ss;
        int temp = 0;
        PTE = PTR + n / 10;
        if (M[PTE] == "")
        {
            PI = 3;
            MOS();
        }
        else
        {
            ss << M[PTE];
            ss >> temp;
            int r = temp * 10 + n % 10;
            return r;
        }
    }
    // return r;
}
void incrementIC()
{
    if (IC[1] == 9)
    {
        IC[0]++;
        IC[1] = 0;
    }
    else
    {
        IC[1]++;
    }
}

void decrementIC()
{
    if (IC[1] == 0)
    {
        IC[0]--;
        IC[1] = 9;
    }
    else
    {
        IC[1]--;
    }
}

void executeUserProgram()
{
    int n = (IC[0] * 10) + IC[1];
    string instruct = M[ADDRESS_MAP(n)];
    cout << "IC :" << IC[0] << IC[1] << "\n";
    cout << instruct << "\n";
    IR[0] = instruct[0];
    IR[1] = instruct[1];
    IR[2] = instruct[2];
    IR[3] = instruct[3];
    cout << "IR :" << IR[0] << IR[1] << IR[2] << IR[3] << "\n";
    memory_data();
    cout << "Reached here\n";
    exit(0);
    incrementIC();

    if (TTC > TTL)
    {
        terminate(3);
        // exit(0);
        return;
    }
    if (IR[0] == 'G' && IR[1] == 'D')
    {
        cout << "GD executed\n";
        // cout<<"GD executed\n";
        VA = (IR[2] - '0') * 10 + (IR[3] - '0');
        RA = ADDRESS_MAP(VA);
        // cout << "RA :" << RA << "\n";
        SI = 1;
        MOS();
        executeUserProgram();
    }
    else if (IR[0] == 'P' && IR[1] == 'D')
    {
        cout << "PD executed\n";
        VA = (IR[2] - '0') * 10 + (IR[3] - '0');
        RA = ADDRESS_MAP(VA);
        SI = 2;
        MOS();
        executeUserProgram();
    }
    else if (IR[0] == 'H')
    {

        cout << "H executed\n";
        SI = 3;
        MOS();
        TTC++;
        cout << "\nHere\n";
        memory_data();
        exit(0);
        return;
    }
    else if (IR[0] == 'L' && IR[1] == 'R')
    {
        cout << "LR executed\n";
        VA = (IR[2] - '0') * 10 + (IR[3] - '0');
        RA = ADDRESS_MAP(VA);
        cout << "VA :" << VA << "RA :" << RA << "\n";
        string temp = M[RA];

        for (int i = 0; i < 4; i++)
        {
            R[i] = temp[i];
        }
        cout << "R :" << R[0] << R[1] << R[2] << R[3] << "\n";
        TTC++;
        // exit(0);
        executeUserProgram();
    }
    else if (IR[0] == 'S' && IR[1] == 'R')
    {
        cout << "SR executed\n";
        int ir2 = IR[2] - '0';
        int ir3 = IR[3] - '0';
        VA = (ir2 * 10) + ir3;
        RA = ADDRESS_MAP(VA);
        // page fault to be handeled
        string value = "";
        for (int i = 0; i < 4; i++)
        {
            value += R[i];
        }
        M[RA] = value;
        cout << "SR execution done\n";
        TTC++;
        // memory_data();
        // exit(0);
        executeUserProgram();
    }
    else if (IR[0] == 'C' && IR[1] == 'R')
    {
        cout << "CR executed\n";
        // cout<<IR[0]<<IR[1];
        int ir2 = IR[2] - '0';
        int ir3 = IR[3] - '0';

        VA = (ir2 * 10) + ir3;
        RA = ADDRESS_MAP(VA);

        string value = M[RA];
        for (int i = 0; i < 4; i++)
        {
            if (value[i] == R[i])
                C = true;

            else
                C = false;
        }
        TTC++;
        executeUserProgram();
    }
    else if (IR[0] == 'B' && IR[1] == 'T')
    {
        cout << "BT executed\n";
        if (C == true)
        {
            IC[0] = IR[2] - '0';
            IC[1] = IR[3] - '0';
        }
        TTC++;
        executeUserProgram();
    }
    else
    {
        // pi=1 operation error
        cout << "Opcode error\n";
        PI = 1;
        MOS();
        return;
    }
}
void startExecution()
{
    IC[0] = 0;
    IC[1] = 0;
    executeUserProgram();
}

void memory_data()
{

    for (int j = 0; j < M.size(); j++)
    {
        cout << "M[" << j << "] " << M[j] << "\n";
    }
}