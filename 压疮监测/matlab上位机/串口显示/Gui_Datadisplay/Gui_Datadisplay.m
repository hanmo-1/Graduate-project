function varargout = Gui_Datadisplay(varargin)
% GUI_DATADISPLAY MATLAB code for Gui_Datadisplay.fig
%      GUI_DATADISPLAY, by itself, creates a new GUI_DATADISPLAY or raises the existing
%      singleton*.
%
%      H = GUI_DATADISPLAY returns the handle to a new GUI_DATADISPLAY or the handle to
%      the existing singleton*.
%
%      GUI_DATADISPLAY('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in GUI_DATADISPLAY.M with the given input arguments.
%
%      GUI_DATADISPLAY('Property','Value',...) creates a new GUI_DATADISPLAY or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before Gui_Datadisplay_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to Gui_Datadisplay_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help Gui_Datadisplay

% Last Modified by GUIDE v2.5 14-Nov-2020 20:20:03

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @Gui_Datadisplay_OpeningFcn, ...
                   'gui_OutputFcn',  @Gui_Datadisplay_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before Gui_Datadisplay is made visible.
function Gui_Datadisplay_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to Gui_Datadisplay (see VARARGIN)
warning off all;
% 改变窗口左上角的图标为icon.jpg
javaFrame = get(hObject,'JavaFrame');
javaFrame.setFigureIcon(javax.swing.ImageIcon('icon.jpg'));
%% 初始化界面的一些参数和按键的状态用来判断按键是否按下的选择
global COM;
global rate;
COM='COM7';
rate = 115200;
set(handles.ppCOM,'value', 7);
set(handles.ppBaudRates,'value',1);
set(handles.pbCloseSerial,'Enable','off');
set(handles.begin_get,'UserData',0);        %开始采集初始化判断
setappdata(handles.save,'ApplicationData',0);
set(handles.three_D,'UserData',0);          %初始化3D坐标图
setappdata(handles.three_D,'ApplicationData',0);
% Choose default command line output for Gui_Datadisplay
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes Gui_Datadisplay wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = Gui_Datadisplay_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;



function Value_Callback(hObject, eventdata, handles)
% hObject    handle to Value (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of Value as text
%        str2double(get(hObject,'String')) returns contents of Value as a double


% --- Executes during object creation, after setting all properties.
function Value_CreateFcn(hObject, eventdata, handles)
% hObject    handle to Value (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in pbOpenSerial.
function pbOpenSerial_Callback(hObject, eventdata, handles)
% hObject    handle to pbOpenSerial (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
clc
instrreset
global s;
global rate;
global COM;
s=serial(COM);
set(s,'BaudRate',rate);%%%Baud初始化
set(s,'DataBits',8);%%%设置数据长度
set(s,'StopBits',1);%%%设置停止位长度
set(s,'InputBufferSize',1024000);%%%设置输入缓冲区大小为1M
set(s,'timeout',10);%%设置接收时间为20s,默认是10s
set(s,'Terminator','CR');%%设置回车换行为终止符，目的为了处理接收到的数据
set(handles.pbOpenSerial,'Enable','off');
set(handles.pbCloseSerial,'Enable','on');
set(handles.begin_get,'Enable','On');
set(handles.exit_window,'Enable','Off');

%串口事件回调设置
s.BytesAvailableFcnMode='byte';
s.BytesAvailableFcnCount=64; %输入缓冲区存在10个字节触发回调函数
s.BytesAvailableFcn={@EveBytesAvailableFcn,handles};%回调函数的指定
try            %利用try catch语法来判断出错后的判断
fopen(s);%打开串口
%% 这两个语句为了防止第一次使用时读取到13个变量，具体原因没弄明白！！！！？？？
% a=fread(s,6);
% set(handles.editR,'string','');
%% 
get(s);  %得到串口的相关属性信息
catch
    msgbox('串口不存在或者被占用');
    set(handles.pbCloseSerial,'Enable','off');
    set(handles.pbOpenSerial,'Enable','on');
end
fprintf('ceshi_dakaichuankou\n');

%回调函数
function EveBytesAvailableFcn( t,event,handles )
global s;
global n;       %能读取的个数
global out;     %用来存放读取的串口数据
global count;   %用来存放读取的串口数据的个数
% global data0,data1,data2,data3,data4,data5,data6,data7;   %存放要用来作图的数据

%这里设置参数是为了画时间-幅值图操作的变量
global t1;  
global x;  
global m0;  
global ii;
global p0;

%这里设置参数是为了画三维坐标图

fprintf('进入了中断回调函数\n');
n=s.BytesAvailable;
if n
   [out,count] = fread(s,64); 
end
%     plot(handles.axes1,out);  
    % 解析数据
%     if(out(1)==128 && bitxor(bitxor(bitxor(bitxor(out(2),out(3)),out(4)),out(5)),out(6)) == out(7))
%         data2 = dec2bin(out(2),8);
%         data3 = dec2bin(out(3),8);
%         data4 = dec2bin(out(4),8);
%         data5 = dec2bin(out(5),8);
%         data6 = dec2bin(out(6),8);
%         cur_data0 = 0;      % 初始化脉冲值
%         cur_data1 = dec2bin(cur_data0,32);   % 转成二进制，有效位数为32
%         cur_data1(1:4) = data6(5:8);
%         cur_data1(5:11) = data5(2:8);
%         cur_data1(12:18) = data4(2:8);
%         cur_data1(19:25) = data3(2:8);
%         cur_data1(26:32) = data2(2:8);
%         cur_data2 = cur_data1;
%         set(handles.current_data0,'String',bin2dec(cur_data2));
%     end
out=char(out');%将从串口设备中读取到的ASCii码转换为字符型用于显示，a'用于行显示
out0=out(1:4);out1=out(5:8);out2=out(9:12);out3=out(13:16);out4=out(17:20);out5=out(21:24);out6=out(25:28);
out7=out(29:32);out8=out(33:36);out9=out(37:40);out10=out(41:44);out11=out(45:48);out12=out(49:52);out13=out(53:56);out14=out(57:60);
out15=out(61:64);
data0=str2num(out0); data1=str2num(out1);data2=str2num(out2);data3=str2num(out3);data4=str2num(out4);data5=str2num(out5);
data6=str2num(out6);data7=str2num(out7);data8=str2num(out8);data9=str2num(out9);data10=str2num(out10);data11=str2num(out11);
data12=str2num(out12);data13=str2num(out13);data14=str2num(out14);data15=str2num(out15);%将通道1的数字保存在data0中用于作图
set(handles.current_data0,'String',data0);
set(handles.current_data1,'String',data1);
set(handles.current_data2,'String',data2);
set(handles.current_data3,'String',data3);
set(handles.current_data4,'String',data4);
set(handles.current_data5,'String',data5);
set(handles.current_data6,'String',data6);
set(handles.current_data7,'String',data7);
set(handles.current_data8,'String',data8);
set(handles.current_data9,'String',data9);
set(handles.current_data10,'String',data10);
set(handles.current_data11,'String',data11);
set(handles.current_data12,'String',data12);
set(handles.current_data13,'String',data13);
set(handles.current_data14,'String',data14);
set(handles.current_data15,'String',data15);%显示数字在当前值中
% show = strcat('电压为：',out,'V');   %设置输出到显示区的格式，因为不会转换asci码转中文显示
% show = sprintf('%s%s%s','电压为：',out,'V');
% % show = strcat(out,get(handles.editR,'string'));
% str = get(handles.editR,'string');  %这里得到的str是元胞数组，要想将字符串数组连接在一起要用到char()
% str = sprintf('%s',char(str));
% str = [str,show];%用于连接字符串，为了连续显示
% set(handles.editR,'string',str);

% set(handles.editR,'string','');
fprintf('测试\r\n');
    if(getappdata(handles.save,'ApplicationData') ~=0)        % 开始存储数据
        
        File = get(handles.save_path_show,'String');
        fid = fopen(File,'a');
        fprintf(fid,'%d\t',bin2dec(cur_data2));
        fprintf(fid,'\n');
        fclose(fid);
    end
%% 开始绘制曲线图功能
    if(getappdata(handles.begin_get,'ApplicationData')~=0)  % 收到单击采集信号的信息
        t1 = [t1 ii];  
        m0 = [m0 data0];
        set(p0, 'XData',t1,'YData',m0(1,:));    %定义XY的坐标值
%         drawnow
        x = x + 0.1;  
%       axis([x-50 x+100 -1 5]);          %移动坐标绘图 这里会重新画一个图
        set(handles.axes1,'XLim',[x-100 x+100],'YLim',[-5 5]); %移动GUI中的axes1的坐标绘图
        ii=ii+0.1; 
%% 开始绘制三维压力图功能
    if(getappdata(handles.three_D,'ApplicationData')==1)
%         set(handles_bar,'XData',[0,0,0,0],'YData',[0,0,0,0],'ZData',[data1,0,0,0]);
        data=[data0,data1,data2,data3;data4,data5,data6,data7;data8,data9,data10,data11;data12,data13,data14,data15];   %这里是绘制4x4矩阵
%         drawnow
        width = 0.5;
        bar3(data,width); %先画然后配置坐标和标号
        axis([0 5 0 5 0 5]); 
        xlabel('row');ylabel('column');zlabel('voltage');  %初始化压力坐标三维图
%         bar3(data);
    end
        
    end      


% --- Executes on selection change in ppCOM.
function ppCOM_Callback(hObject, eventdata, handles)
% hObject    handle to ppCOM (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns ppCOM contents as cell array
%        contents{get(hObject,'Value')} returns selected item from ppCOM
global COM;
% COM=1;
rate=1;
val=get(hObject,'value');
switch val
    case 1
        COM='COM1';
        fprintf('ceshi_COM=1\n');
    case 2
        COM='COM2';
    case 3
        COM='COM3';
    case 4
        COM='COM4';
    case 5
        COM='COM5';
    case 6
        COM='COM6';
    case 7
        COM='COM7';
        fprintf('ceshi_COM=7\n');
    case 8
        COM='COM8';
    case 9
        COM='COM9';
end

% --- Executes during object creation, after setting all properties.
function ppCOM_CreateFcn(hObject, eventdata, handles)
% hObject    handle to ppCOM (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called


% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on selection change in ppBaudRates.
function ppBaudRates_Callback(hObject, eventdata, handles)
% hObject    handle to ppBaudRates (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns ppBaudRates contents as cell array
%        contents{get(hObject,'Value')} returns selected item from ppBaudRates
global rate;
val=get(hObject,'value');
switch val
    case 1
        rate=115200;
    case 2
        rate=9600;
end 

% --- Executes during object creation, after setting all properties.
function ppBaudRates_CreateFcn(hObject, eventdata, handles)
% hObject    handle to ppBaudRates (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in pbCloseSerial.
function pbCloseSerial_Callback(hObject, eventdata, handles)
% hObject    handle to pbCloseSerial (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global s;
fclose(s);
delete(s);
set(handles.pbOpenSerial,'Enable','on');
set(handles.pbCloseSerial,'Enable','off');
setappdata(handles.axes1,'ApplicationData',0);  % 清空缓存数据
set(handles.axes1,'UserData',0);                % 将计数器清0
set(handles.save,'UserData',0);                 % 将数据保存区清0
set(handles.begin_get,'Enable','Off');
set(handles.save,'Enable','Off');
set(handles.exit_window,'Enable','On');

fprintf('Close_COM');


function editR_Callback(hObject, eventdata, handles)
% hObject    handle to editR (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of editR as text
%        str2double(get(hObject,'String')) returns contents of editR as a double


% --- Executes during object creation, after setting all properties.
function editR_CreateFcn(hObject, eventdata, handles)
% hObject    handle to editR (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in pbClearR.
function pbClearR_Callback(hObject, eventdata, handles)
% hObject    handle to pbClearR (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(handles.editR,'string','');




% --- Executes on button press in pbClearT.
function pbClearT_Callback(hObject, eventdata, handles)
% hObject    handle to pbClearT (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(handles.editT,'string','');

% --- Executes on button press in pbSend.
function pbSend_Callback(hObject, eventdata, handles)
% hObject    handle to pbSend (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global d;
d=get(handles.editT,'String');
set(handles.Value,'string',d);


function editT_Callback(hObject, eventdata, handles)
% hObject    handle to editT (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of editT as text
%        str2double(get(hObject,'String')) returns contents of editT as a double


% --- Executes during object creation, after setting all properties.
function editT_CreateFcn(hObject, eventdata, handles)
% hObject    handle to editT (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes during object creation, after setting all properties.
function axes1_CreateFcn(hObject, eventdata, handles)
% hObject    handle to axes1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: place code in OpeningFcn to populate axes1


% --- Executes on mouse press over axes background.
function axes1_ButtonDownFcn(hObject, eventdata, handles)
% hObject    handle to axes1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on key press with focus on editR and none of its controls.
function editR_KeyPressFcn(hObject, eventdata, handles)
% hObject    handle to editR (see GCBO)
% eventdata  structure with the following fields (see MATLAB.UI.CONTROL.UICONTROL)
%	Key: name of the key that was pressed, in lower case
%	Character: character interpretation of the key(s) that was pressed
%	Modifier: name(s) of the modifier key(s) (i.e., control, shift) pressed
% handles    structure with handles and user data (see GUIDATA)


% --- Executes during object deletion, before destroying properties.
function editR_DeleteFcn(hObject, eventdata, handles)
% hObject    handle to editR (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- If Enable == 'on', executes on mouse press in 5 pixel border.
% --- Otherwise, executes on mouse press in 5 pixel border or over editR.
function editR_ButtonDownFcn(hObject, eventdata, handles)
% hObject    handle to editR (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- If Enable == 'on', executes on mouse press in 5 pixel border.
% --- Otherwise, executes on mouse press in 5 pixel border or over pbClearT.
function pbClearT_ButtonDownFcn(hObject, eventdata, handles)
% hObject    handle to pbClearT (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on button press in save_path.
function save_path_Callback(hObject, eventdata, handles)
% hObject    handle to save_path (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(handles.save,'Enable','On');
[file, pathname, index] = uiputfile({'*.txt';'*.dat';'*.mat'},'文件另存为');
file1 = strcat(pathname,file);
set(handles.save_path_show,'String',file1);
set(handles.save_path,'UserData',index);      % 判断是否设定了保存文件的路径


% --- Executes on button press in save.
function save_Callback(hObject, eventdata, handles)
% hObject    handle to save (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
  setappdata(handles.save,'ApplicationData',1);

% --- Executes on button press in begin_get.
function begin_get_Callback(hObject, eventdata, handles)
% hObject    handle to begin_get (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
index = get(handles.save_path,'UserData');         % 是否设定存储路径
global count;
global t1;  
global x;  
global m0;  
global ii;
global p0;
                                      
if ( count==0 )
    msgbox('当前无数据输入！');
    set(handles.save,'Enable','Off');
else
    if(get(handles.begin_get,'UserData')==0)    %UserData用来表示按钮中开始采集和停止采集切换
        set(handles.begin_get,'String','停止采集');  %说明已经在采集了
        set(handles.clear_axes1,'Enable','Off');       %采集的数据的时候不能清除坐标数据
        set(handles.begin_get,'BackgroundColor',[1 0 0]);
        setappdata(handles.begin_get,'ApplicationData',1);   % 用于传递给中断响应函数，可以接收并显示数据了
        set(handles.begin_get,'UserData',1);
        if(  index~=0 )                                       % 设定存储条件
            set(handles.save,'Enable','On');
        else
            set(handles.save,'Enable','Off');
        end
        %  绘图初始化 
        t1 = [0];  
        m0 = [0];m1 = [0];m2 = [0];m3 = [0];m4 = [0];m5 = [0];m6 = [0];m7 = [0];
        ii = 0;  
        x = 0;
        
        p0 = plot(t1,m0,'r','LineWidth',1,...
            'EraseMode','background','MarkerSize',1);%p为绘图句柄
        xlabel('时间');
        ylabel('信号');
        axis([x-100 x+100 -5 5]);                                %初始化绘图面板
        grid on;   
    else
        set(handles.begin_get,'String','开始采集');%说明已经暂停采集了
        set(handles.clear_axes1,'Enable','on');%在开始采集的时候清除坐标键可以用
        setappdata(handles.begin_get,'ApplicationData',0); %Application一般用来将判断的传到其他函数中这里是传输中断回调函数
        set(handles.begin_get,'BackgroundColor',[0.8 0.8 0.8]);
        set(handles.begin_get,'UserData',0);
        set(handles.save,'Enable','Off');
    end

 
    
end


% --- Executes during object creation, after setting all properties.
function current_data0_CreateFcn(hObject, eventdata, handles)
% hObject    handle to current_data0 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in exit_window.
function exit_window_Callback(hObject, eventdata, handles)
% hObject    handle to exit_window (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
close(gcbf);


% --- Executes on button press in clear_axes1.
function clear_axes1_Callback(hObject, eventdata, handles)
% hObject    handle to clear_axes1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
delete(allchild(handles.axes1));%清除坐标数据


% --- Executes on button press in three_D.
function three_D_Callback(hObject, eventdata, handles)
% hObject    handle to three_D (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global h1;
global handles_bar;
if(get(handles.three_D,'UserData')==0)
   %% 绘图初始化
    h1 = figure(1);
%     axes2=get(h1,'CurrentAxes');
    data1 = [0,0,0,0;0,0,0,0;0,0,0,0;0,0,0,0];   %这里是4x4矩阵
    width = 0.5;
    handles_bar = bar3(data1,width);
    axis([0 5 0 5 0 5]); 
    xlabel('row');ylabel('column');zlabel('voltage');  %初始化压力坐标三维图
%     set(axes2,'XLim',[1 4],'YLim',[1 4],'ZLim',[0 5]);
%     axis([1 4 1 4 0 5]); 
    title('压力图');
%% 这里是编辑按键控制切换功能
    set(handles.three_D,'string','关闭三维柱状图');
    set(handles.three_D,'UserData',1);
    set(handles.pbCloseSerial,'Enable','off');   %防止按键后出现画图在整个GUI图上
    setappdata(handles.three_D,'ApplicationData',1);   % 用于判断按键是否按下然后在中断函数中处理来动态显示三维柱状图
    

else
    setappdata(handles.three_D,'ApplicationData',0);
    set(handles.three_D,'string','绘制三维柱状图');
    set(handles.pbCloseSerial,'Enable','on');
    set(handles.three_D,'UserData',0);
    close(h1);
end



% --- Executes during object creation, after setting all properties.
function three_D_CreateFcn(hObject, eventdata, handles)
% hObject    handle to three_D (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called


% --- Executes on button press in pressure_show.
% function pressure_show_Callback(hObject, eventdata, handles)
% % hObject    handle to pressure_show (see GCBO)
% % eventdata  reserved - to be defined in a future version of MATLAB
% % handles    structure with handles and user data (see GUIDATA)
% global h2;
% global handles_pcolor;
% if(get(handles.pressure_show,'UserData')==0)
%    %% 绘图初始化
%     h2 = figure(2);
% %     axes2=get(h1,'CurrentAxes');
%     data1 = [0,0,0,0;0,0,0,0;0,0,0,0;0,0,0,0];   %这里是4x4矩阵
%     handles_bar = bar3(data1);
%     axis([0 5 0 5 0 5]); 
%     xlabel('row');ylabel('column');zlabel('voltage');  %初始化压力坐标三维图
% %     set(axes2,'XLim',[1 4],'YLim',[1 4],'ZLim',[0 5]);
% %     axis([1 4 1 4 0 5]); 
%     title('压力彩色图图');
% %% 这里是编辑按键控制切换功能
%     set(handles.pressure_show,'string','关闭伪彩色图');
%     set(handles.pressure_show,'UserData',1);
%     set(handles.pbCloseSerial,'Enable','off');   %防止按键后出现画图在整个GUI图上
%     setappdata(handles.pressure_show,'ApplicationData',1);   % 用于判断按键是否按下然后在中断函数中处理来动态显示三维柱状图
%     
% 
% else
%     setappdata(handles.pressure_show,'ApplicationData',0);
%     set(handles.pressure_show,'string','绘制伪彩色图');
%     set(handles.pbCloseSerial,'Enable','on');
%     set(handles.pressure_show,'UserData',0);
%     close(h2);
% end



function current_data1_Callback(hObject, eventdata, handles)
% hObject    handle to current_data1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of current_data1 as text
%        str2double(get(hObject,'String')) returns contents of current_data1 as a double


% --- Executes during object creation, after setting all properties.
function current_data1_CreateFcn(hObject, eventdata, handles)
% hObject    handle to current_data1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function current_data2_Callback(hObject, eventdata, handles)
% hObject    handle to current_data2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of current_data2 as text
%        str2double(get(hObject,'String')) returns contents of current_data2 as a double


% --- Executes during object creation, after setting all properties.
function current_data2_CreateFcn(hObject, eventdata, handles)
% hObject    handle to current_data2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function current_data3_Callback(hObject, eventdata, handles)
% hObject    handle to current_data3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of current_data3 as text
%        str2double(get(hObject,'String')) returns contents of current_data3 as a double


% --- Executes during object creation, after setting all properties.
function current_data3_CreateFcn(hObject, eventdata, handles)
% hObject    handle to current_data3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function current_data4_Callback(hObject, eventdata, handles)
% hObject    handle to current_data4 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of current_data4 as text
%        str2double(get(hObject,'String')) returns contents of current_data4 as a double


% --- Executes during object creation, after setting all properties.
function current_data4_CreateFcn(hObject, eventdata, handles)
% hObject    handle to current_data4 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function current_data5_Callback(hObject, eventdata, handles)
% hObject    handle to current_data5 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of current_data5 as text
%        str2double(get(hObject,'String')) returns contents of current_data5 as a double


% --- Executes during object creation, after setting all properties.
function current_data5_CreateFcn(hObject, eventdata, handles)
% hObject    handle to current_data5 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function current_data6_Callback(hObject, eventdata, handles)
% hObject    handle to current_data6 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of current_data6 as text
%        str2double(get(hObject,'String')) returns contents of current_data6 as a double


% --- Executes during object creation, after setting all properties.
function current_data6_CreateFcn(hObject, eventdata, handles)
% hObject    handle to current_data6 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function current_data7_Callback(hObject, eventdata, handles)
% hObject    handle to current_data7 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of current_data7 as text
%        str2double(get(hObject,'String')) returns contents of current_data7 as a double


% --- Executes during object creation, after setting all properties.
function current_data7_CreateFcn(hObject, eventdata, handles)
% hObject    handle to current_data7 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function current_data8_Callback(hObject, eventdata, handles)
% hObject    handle to current_data8 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of current_data8 as text
%        str2double(get(hObject,'String')) returns contents of current_data8 as a double


% --- Executes during object creation, after setting all properties.
function current_data8_CreateFcn(hObject, eventdata, handles)
% hObject    handle to current_data8 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function current_data9_Callback(hObject, eventdata, handles)
% hObject    handle to current_data9 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of current_data9 as text
%        str2double(get(hObject,'String')) returns contents of current_data9 as a double


% --- Executes during object creation, after setting all properties.
function current_data9_CreateFcn(hObject, eventdata, handles)
% hObject    handle to current_data9 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function current_data14_Callback(hObject, eventdata, handles)
% hObject    handle to current_data14 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of current_data14 as text
%        str2double(get(hObject,'String')) returns contents of current_data14 as a double


% --- Executes during object creation, after setting all properties.
function current_data14_CreateFcn(hObject, eventdata, handles)
% hObject    handle to current_data14 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function current_data10_Callback(hObject, eventdata, handles)
% hObject    handle to current_data10 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of current_data10 as text
%        str2double(get(hObject,'String')) returns contents of current_data10 as a double


% --- Executes during object creation, after setting all properties.
function current_data10_CreateFcn(hObject, eventdata, handles)
% hObject    handle to current_data10 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function current_data11_Callback(hObject, eventdata, handles)
% hObject    handle to current_data11 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of current_data11 as text
%        str2double(get(hObject,'String')) returns contents of current_data11 as a double


% --- Executes during object creation, after setting all properties.
function current_data11_CreateFcn(hObject, eventdata, handles)
% hObject    handle to current_data11 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function current_data12_Callback(hObject, eventdata, handles)
% hObject    handle to current_data12 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of current_data12 as text
%        str2double(get(hObject,'String')) returns contents of current_data12 as a double


% --- Executes during object creation, after setting all properties.
function current_data12_CreateFcn(hObject, eventdata, handles)
% hObject    handle to current_data12 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function current_data13_Callback(hObject, eventdata, handles)
% hObject    handle to current_data13 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of current_data13 as text
%        str2double(get(hObject,'String')) returns contents of current_data13 as a double


% --- Executes during object creation, after setting all properties.
function current_data13_CreateFcn(hObject, eventdata, handles)
% hObject    handle to current_data13 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function current_data15_Callback(hObject, eventdata, handles)
% hObject    handle to current_data15 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of current_data15 as text
%        str2double(get(hObject,'String')) returns contents of current_data15 as a double


% --- Executes during object creation, after setting all properties.
function current_data15_CreateFcn(hObject, eventdata, handles)
% hObject    handle to current_data15 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
