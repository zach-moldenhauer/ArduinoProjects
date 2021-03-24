#include <Windows.h>
#include <iostream>

using namespace std;


void readFromCOM(HANDLE hComm){
	
	//Create listener to monitor COM4 connection
	SetCommMask(hComm, EV_RXCHAR);  //EV_RXCHAR is listening for a character
	
	//Stop and wait until char is recieved
	DWORD dwEventMask;
	WaitCommEvent(hComm, &dwEventMask, NULL);
	
	//After char is recieved
	unsigned char TempChar; //Temp char used for reading
	char SerialBuffer[255]; //Buffer for storing Rxed Data
	DWORD NumberBytesRead;
	int i = 0;
	
	
	ReadFile( hComm,
				  &SerialBuffer,
				  sizeof(SerialBuffer),
				  &NumberBytesRead,
				  NULL);
				  
	
	/* do
	{
		ReadFile( hComm,
				  &SerialBuffer,
				  sizeof(SerialBuffer),
				  &NumberBytesRead,
				  NULL);
				  
		SerialBuffer[i] = TempChar; //Store char into buffer
		//cout << "char: " << TempChar << endl;
		i++;
	} while(i < 5);
	 */
	

	cout << "buffer: " << SerialBuffer << endl;
	
}

//returns 1 if successfull
int writeToCOM(HANDLE hComm, string input){
	char* buffer = const_cast<char*>(input.c_str());
	
	DWORD numberOfBytesToWrite;
	DWORD numberOfBytesWritten;
	numberOfBytesToWrite = sizeof(buffer);

	WriteFile(hComm,
			  buffer,
			  numberOfBytesToWrite,
			  &numberOfBytesWritten,
			  NULL);
			  
	if(numberOfBytesWritten == numberOfBytesToWrite) {
		return 1;
		
	} else {
		return 0;
	}
	
}

HANDLE setup(string input) {
	
	//open connection to COM4
	HANDLE hComm;
	hComm = CreateFile( input.c_str(),  
						GENERIC_READ | GENERIC_WRITE, 
						0, 
						0, 
						OPEN_EXISTING,
						0,
						0);
	
	
	//Create DCB structure with necessary settings
	DCB dcbSerialParams = { 0 }; //initialize DCB
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	
	GetCommState(hComm, &dcbSerialParams);
	dcbSerialParams.BaudRate = CBR_9600;
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.StopBits = ONESTOPBIT;
	dcbSerialParams.Parity = NOPARITY;
	
	//Set DCB settings
	SetCommState(hComm, &dcbSerialParams);

	//Timeout Settings
	COMMTIMEOUTS timeouts = { 0 };
	timeouts.ReadIntervalTimeout         = 50; // in milliseconds
	timeouts.ReadTotalTimeoutConstant    = 50; // in milliseconds
	timeouts.ReadTotalTimeoutMultiplier  = 10; // in milliseconds
	timeouts.WriteTotalTimeoutConstant   = 50; // in milliseconds
	timeouts.WriteTotalTimeoutMultiplier = 10; // in milliseconds
	
	//Set Timout settings
	SetCommTimeouts(hComm, &timeouts);
	
	return hComm;
	
}


int main() {
	
	
	HANDLE hComm;
	bool runProgram = true;
	bool automatic = false;
    string connection = "No Connection";
    string input;
	string dispenseAmount;
	
	while(runProgram) {
		cout << "" << endl;
		cout << "--- String Dispenser: menu ---" << endl;
		cout << "CONNECTION: " << connection << endl;
		cout << "COMMANDS: /setup to configure usb connection" << endl;
		cout << "          /run to begin dispensing process" << endl;
		cout << "          /quit to close program" << endl;
		cout << "" << endl;
		cout << ">>>";
		
		//take in user input and assign to dynamic memory
		cin >> input;
		
		if(input == "/setup"){
			cout << "--- String Dispenser: setup ---" << endl;
			cout << "Enter COM port: ";
			cin >> connection;
			
			hComm = setup(connection);
			
			if (hComm == INVALID_HANDLE_VALUE){
				cout << "Failed Connection" << endl;
				connection = "No Connection";
				CloseHandle(hComm);

			} else {
				cout << "Successful Connection" << endl;
			}
			
			cout << "Multiple runs? y or n:";
			string in;
			cin >> in;
			
			if(in == "y") {
		        automatic = true;
				
			} else {
                automatic = false;

			}
		}
		if(input == "/run"){
			
			do {
			
				cout << "--- String Dispenser: run ---" << endl;
				cout << "Auto: " << (automatic ? "true" : "false") << endl;
				
				if(automatic) {
					cout << "To exit Auto loop type /quit" << endl;
				}
				cout << "Enter amount (ft) to dispense: ";
				cin >> dispenseAmount;
				
				if(dispenseAmount == "/quit") {
					break;
				}
				
				if(writeToCOM(hComm, dispenseAmount)) {
					cout << "Successfully written to COM" << endl;
					
				} else {
					cout << "Failed write to COM" << endl;
				}				
			} while(automatic);
			
		}
		if(input == "/quit"){
			runProgram = false;
			CloseHandle(hComm);
		}
		
	}
		
	return 0;
}