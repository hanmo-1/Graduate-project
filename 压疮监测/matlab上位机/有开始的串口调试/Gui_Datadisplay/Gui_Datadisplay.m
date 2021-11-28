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

% Last Modified by GUIDE v2.5 06-Nov-2020 11:29:30

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

global COM;
global rate;
COM='COM7';
rate = 115200;
set(handles.ppCOM,'value', 7);
set(handles.ppBaudRates,'value',1);
set(handles.pbCloseSerial,'Enable','off');
set(handles.begin_get,'UserData',0);
setappdata(handles.save,'ApplicationData',0);
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
global out;
out=1;
s=serial(COM);
set(s,'BaudRate',rate);%%%Baud初始化
set(s,'DataBits',8);%%%设置数据长度
set(s,'StopBits',1);%%%设置停止位长度
set(s,'InputBufferSize',1024000);%%%设置输入缓冲区大小为1M
set(s,'timeout',10);%%设置接收时间为20s,默认是10s
set(s,'Terminator',10);%%设置回车换行为终止符，目的为了处理接收到的数据
set(handles.pbOpenSerial,'Enable','off');
set(handles.pbCloseSerial,'Enable','on');
set(handles.begin_get,'Enable','On');
set(handles.exit_window,'Enable','Off');

%串口事件回调设置
s.BytesAvailableFcnMode='byte';
s.BytesAvailableFcnCount=6; %输入缓冲区存在10个字节触发回调函数
s.BytesAvailableFcn={@EveBytesAvailableFcn,handles};%回调函数的指定
try            %利用try catch语法来判断出错后的判断
fopen(s);%打开串口
get(s);  %得到串口的相关属性信息
catch
    msgbox('串口不存在或者被占用');
end
fprintf('ceshi_dakaichuankou\n');

%回调函数
function EveBytesAvailableFcn( t,event,handles )
global s;
global n;
global out;
global count;
fprintf('进入了中断回调函数\n');
n=s.BytesAvailable;
if(getappdata(handles.begin_get,'ApplicationData')~=0)            % 收到单击采集信号的信息
if n
   [out,count] = fread(s,6);                       % 读取数据
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
%         set(handles.current_data,'String',bin2dec(cur_data2));
%     end
out=char(out');%将从串口设备中读取到的ASCii码转换为字符型用于显示，a'用于行显示
data1=str2num(out); %将通道1的数字保存在data1中用于作图
set(handles.current_data,'String',data1);  %显示数字在当前值中
show = strcat('电压为：',out,'V');   %设置输出到显示区的格式，因为不会转换asci码转中文显示
out = strcat(get(handles.editR,'string'),show);
set(handles.editR,'string',out);
fprintf('测试\r\n');
    if(getappdata(handles.save,'ApplicationData') ~=0)        % 开始存储数据
        
        File = get(handles.save_path_show,'String');
        fid = fopen(File,'a');
        fprintf(fid,'%d\t',bin2dec(cur_data2));
        fprintf(fid,'\n');
        fclose(fid);
        
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
set(handles.Value,'string','');
set(handles.text4,'string','');


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


% --- Executes on button press in begin_get.
function begin_get_Callback(hObject, eventdata, handles)
% hObject    handle to begin_get (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
index = get(handles.save_path,'UserData');         % 是否设定存储路径
global count;
global s;
[a,count] = fread(s,6);                       % 读取数据
if ( count==0 )
    msgbox('当前无数据输入！');
    set(handles.save,'Enable','Off');
else
    if(get(handles.begin_get,'UserData')==0)    %UserData用来表示按钮中开始采集和停止采集切换
        set(handles.begin_get,'String','停止采集');
        set(handles.begin_get,'BackgroundColor',[1 0 0]);
        setappdata(handles.begin_get,'ApplicationData',1);   % 用于传递给中断响应函数，可以接收并显示数据了
        set(handles.begin_get,'UserData',1);
        if(  index~=0 )                                       % 设定存储条件
            set(handles.save,'Enable','On');
        else
            set(handles.save,'Enable','Off');
        end
    else
        set(handles.begin_get,'String','开始采集');
        setappdata(handles.begin_get,'ApplicationData',0); 
        set(handles.begin_get,'BackgroundColor',[0.8 0.8 0.8]);
        set(handles.begin_get,'UserData',0);
        set(handles.save,'Enable','Off');
    end
    
end


% --- Executes during object creation, after setting all properties.
function current_data_CreateFcn(hObject, eventdata, handles)
% hObject    handle to current_data (see GCBO)
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
