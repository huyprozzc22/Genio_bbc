import tkinter as tk

class WindowsForm(tk.Tk):
    __startingNumberRows = 2
    __dataRows = [] 

    def __init__(self, BtnGetIDFunc, BtnStartFunc):
        super().__init__() 

        if (self.winfo_screenwidth() == 1536) and (self.winfo_screenheight() == 864): 
            self.screenWidth = self.winfo_screenwidth()
            self.screenHeight = self.winfo_screenheight()
        elif (self.winfo_screenwidth() == 1920) and (self.winfo_screenheight() == 1080): 
            self.screenWidth = self.winfo_screenwidth()
            self.screenHeight = self.winfo_screenheight()

        print('Chiều dài màn hình: ' + str(self.screenWidth) + ', Chiều cao màn hình: ' + str(self.screenHeight))
        self.title('Tester GUI') 
        #self.geometry('%dx%d'%(self.screenWidth, self.screenHeight)) 
        self.geometry('%dx%d'%(1400, 500)) 

        self.getInputID = tk.IntVar()

        self.on_startGUI(cmdGetID=BtnGetIDFunc, cmdStart=BtnStartFunc)  

    def on_startGUI(self, cmdGetID, cmdStart): 
        settingFrame = tk.LabelFrame(
                self, 
                text='Settings',
                font=('Arial', 14))  
        settingFrame.place(x=10, y=20, width=400, height=100) 
        
        self.textInput = tk.Entry(
            settingFrame, 
            textvariable=self.getInputID, 
            font=('Arial', 14),
            justify='center', 
            borderwidth=3)
        self.textInput.place(x=20, y=20, width=80, height=30) 
        
        tk.Button(
            settingFrame, 
            text='Get ID',  
            font=('Arial', 14), 
            relief='raised',   
            border=3, 
            command=cmdGetID).place(x=110, y=20, width=80, height=30) 
        
        tk.Button(
            settingFrame, 
            text='Start',  
            font=('Arial', 14), 
            relief='raised',   
            border=3, 
            command=cmdStart).place(x=200, y=20, width=80, height=30) 
        
        self.tableFrame = tk.Frame(self)
        self.tableFrame.place(x=10, y=150, height=300) 

        col_0 = tk.StringVar()
        col_0.set('ID') 
        tk.Entry(
            self.tableFrame, 
            textvariable=col_0,
            width=20, 
            justify='center',
            font=('Arial', 11),
            state='readonly').grid(row=1, column=0) 
        
        col_1 = tk.StringVar()
        col_1.set('Bước chân') 
        tk.Entry(
            self.tableFrame, 
            textvariable=col_1,
            width=20, 
            justify='center',
            font=('Arial', 11),
            state='readonly').grid(row=1, column=1) 

        col_2 = tk.StringVar()
        col_2.set('Kinh độ') 
        tk.Entry(
            self.tableFrame, 
            textvariable=col_2,
            width=20, 
            justify='center',
            font=('Arial', 11),
            state='readonly').grid(row=1, column=2) 
        
        col_3 = tk.StringVar()
        col_3.set('Vĩ độ') 
        tk.Entry(
            self.tableFrame, 
            textvariable=col_3,
            width=20, 
            justify='center',
            font=('Arial', 11),
            state='readonly').grid(row=1, column=3) 
        
        col_4 = tk.StringVar()
        col_4.set('Phần trăm pin') 
        tk.Entry(
            self.tableFrame, 
            textvariable=col_4,
            width=20, 
            justify='center',
            font=('Arial', 11),
            state='readonly').grid(row=1, column=4) 
        
        col_5 = tk.StringVar()
        col_5.set('Khả năng tăng tốc') 
        tk.Entry(
            self.tableFrame, 
            textvariable=col_5,
            width=20, 
            justify='center',
            font=('Arial', 11),
            state='readonly').grid(row=1, column=5) 
        
        col_6 = tk.StringVar()
        col_6.set('Trạng thái sử dụng')  
        tk.Entry(
            self.tableFrame, 
            textvariable=col_6,
            width=20, 
            justify='center',
            font=('Arial', 11),
            state='readonly').grid(row=1, column=6) 

        col_7 = tk.StringVar()
        col_7.set('Thời gian hoạt động')   
        tk.Entry(
            self.tableFrame, 
            textvariable=col_7,
            width=20, 
            justify='center',
            font=('Arial', 11),
            state='readonly').grid(row=1, column=7) 
        
    def drawTable(self, dataList):
        items = [] 

        self.__startingNumberRows = self.__startingNumberRows + 1
        for i in range(len(dataList)):
            cell = tk.Entry(
                self.tableFrame, 
                textvariable=dataList[i],
                font=('Arial', 11),
                justify='center') 
            cell.grid(row=self.__startingNumberRows, column=i)  

            items.append(cell) 

        #self.__dataRows.append(items) 
