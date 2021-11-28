function varargout = test3(varargin)
% TEST3 MATLAB code for test3.fig
%      TEST3, by itself, creates a new TEST3 or raises the existing
%      singleton*.
%
%      H = TEST3 returns the handle to a new TEST3 or the handle to
%      the existing singleton*.
%
%      TEST3('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in TEST3.M with the given input arguments.
%
%      TEST3('Property','Value',...) creates a new TEST3 or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before test3_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to test3_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help test3

% Last Modified by GUIDE v2.5 06-Mar-2019 11:54:30

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @test3_OpeningFcn, ...
                   'gui_OutputFcn',  @test3_OutputFcn, ...
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


% --- Executes just before test3 is made visible.
function test3_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to test3 (see VARARGIN)

% Choose default command line output for test3
handles.output = hObject;
warning('off');
javaFrame = get(hObject, 'JavaFrame');
javaFrame.setFigureIcon(javax.swing.ImageIcon('icon.jpg'));
%% 获取图像数据
[num,x,y,z]=textread('chuanlieyi2.txt','%d %f %f %f');
front=1;
back=1;
up=1;
down=1;
for i=1:1:size(num)
if x(i)>500
    front_point(1,front)=x(i);
    front_point(2,front)=y(i);
    front_point(3,front)=z(i);
    front=front+1;
elseif x(i)<-500
    back_point(1,back)=x(i);
    back_point(2,back)=y(i);
    back_point(3,back)=z(i);
    back=back+1;
elseif z(i)>41
    top_point(1,up)=x(i);
    top_point(2,up)=y(i);
    top_point(3,up)=z(i);
    up=up+1;
else
    down_point(1,down)=x(i);
    down_point(2,down)=y(i);
    down_point(3,down)=z(i);
    down=down+1;  
end
end
%% 初始化参数
hasData = false; %表征串口是否接收到数据
isShow = false;  %表征是否正在进行数据显示，即是否正在执行函数dataDisp
isStopDisp = false;  %表征是否按下了【停止显示】按钮
isHexDisp = false;   %表征是否勾选了【十六进制显示】
isHexSend = false;   %表征是否勾选了【十六进制发送】
numRec = 0;    %接收字符计数
numSend = 0;   %发送字符计数
strRec = '';   %已接收的字符串
pitch=0;
roll=0;
yaw=0;
mag_x=0;
mag_y=0;
mag_z=0;
[num,x,y,z]=textread('chuanlieyi2.txt','%d %f %f %f');  %导入串列翼点坐标信息
front=1;
back=1;
up=1;
down=1;
for i=1:1:size(num)
if x(i)>500
    front_point(1,front)=x(i);
    front_point(2,front)=y(i);
    front_point(3,front)=z(i);
    front=front+1;
elseif x(i)<-500
    back_point(1,back)=x(i);
    back_point(2,back)=y(i);
    back_point(3,back)=z(i);
    back=back+1;
elseif z(i)>41
    top_point(1,up)=x(i);
    top_point(2,up)=y(i);
    top_point(3,up)=z(i);
    up=up+1;
else
    down_point(1,down)=x(i);
    down_point(2,down)=y(i);
    down_point(3,down)=z(i);
    down=down+1;  
end
end
global fly_front fly_back fly_top fly_down bar mag_point GPS_longitude GPS_latitude GPS_altitude;

fly_front=front_point;          %画图区分色块，将串列翼点信息分类
fly_back=back_point;
fly_top=top_point;
fly_down=down_point;
bar=[0];                        %存储的数据
mag_point=[0 0 0];
GPS_longitude=[];
GPS_latitude=[];
GPS_altitude=[];
%% 将上述参数作为应用数据，存入窗口对象内       %设置窗口数据绑定，类似
setappdata(hObject, 'hasData', hasData);
setappdata(hObject, 'strRec', strRec);
setappdata(hObject, 'numRec', numRec);
setappdata(hObject, 'numSend', numSend);
setappdata(hObject, 'isShow', isShow);
setappdata(hObject, 'isStopDisp', isStopDisp);
setappdata(hObject, 'isHexDisp', isHexDisp);
setappdata(hObject, 'isHexSend', isHexSend);
% setappdata(hObject, 'str_new', str_new);
% setappdata(hObject, 'str_num', str_num);
setappdata(hObject, 'pitch', pitch);
setappdata(hObject, 'roll', roll);
setappdata(hObject, 'yaw', yaw);
setappdata(hObject, 'front_point', front_point);
setappdata(hObject, 'back_point', back_point);
setappdata(hObject, 'top_point', top_point);
setappdata(hObject, 'down_point', down_point);
global up_data;
up_data=0;
% Update handles structure
guidata(hObject, handles);

% UIWAIT makes test3 wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = test3_OutputFcn(hObject, eventdata, handles)  
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on selection change in serial_port_name.
function serial_port_name_Callback(hObject, eventdata, handles)   %串口回调函数
% hObject    handle to serial_port_name (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
com_str=get(hObject,'string');
com_num=get(hObject,'value');
s=serial(char(com_str(com_num,:)));
handles.serial_port=s;
guidata(hObject,handles);
% Hints: contents = cellstr(get(hObject,'String')) returns serial_port_name contents as cell array
%        contents{get(hObject,'Value')} returns selected item from serial_port_name


% --- Executes during object creation, after setting all properties.
function serial_port_name_CreateFcn(hObject, eventdata, handles)
% hObject    handle to serial_port_name (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
set(hObject,'position',[1,4,12,2],...
            'fontsize',9);
s=serial('com1');
handles.serial_port=s;
guidata(hObject,handles);
% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on selection change in serial_port_baudrate.
function serial_port_baudrate_Callback(hObject, eventdata, handles)
% hObject    handle to serial_port_baudrate (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
s=handles.serial_port;
com_str=get(hObject,'string');
com_num=get(hObject,'value');
set(s,'BaudRate',str2num(char(com_str(com_num,:))));
handles.serial_port=s;
guidata(hObject,handles);
% Hints: contents = cellstr(get(hObject,'String')) returns serial_port_baudrate contents as cell array
%        contents{get(hObject,'Value')} returns selected item from serial_port_baudrate


% --- Executes during object creation, after setting all properties.
function serial_port_baudrate_CreateFcn(hObject, eventdata, handles)
% hObject    handle to serial_port_baudrate (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
set(hObject,'position',[1,1,12,2],...
            'fontsize',9);
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

function ReceiveCallback( obj,event, handles) 
%%% 获取参数
strRec = getappdata(handles.figure1, 'strRec'); %获取串口要显示的数据
numRec = getappdata(handles.figure1, 'numRec'); %获取串口已接收数据的个数
isStopDisp = getappdata(handles.figure1, 'isStopDisp'); %是否按下了【停止显示】按钮
isHexDisp = getappdata(handles.figure1, 'isHexDisp'); %是否十六进制显示
isShow = getappdata(handles.figure1, 'isShow');  %是否正在执行显示数据操作
%% 若正在执行数据显示操作，暂不接收串口数据
if isShow
    return;
end

%% 获取串口可获取的数据个数
n = get(obj, 'BytesAvailable');
%% 若串口有数据，接收所有数据
if n
    %% 更新hasData参数，表明串口有数据需要显示
    setappdata(handles.figure1, 'hasData', true);
    %% 读取串口数据
    a = fread(obj, n, 'char');
    %% 若没有停止显示，将接收到的数据解算出来，准备显示
    if ~isStopDisp 
        %% 根据进制显示的状态，解析数据为要显示的字符串
        if ~isHexDisp 
            c = char(a');
        else
            strHex = dec2hex(a')';
            strHex2 = [strHex; blanks(size(a, 1))];
            c = strHex2(:)';
        end
        %% 更新已接收的数据个数
        numRec = numRec + size(a, 1);
        %% 更新要显示的字符串
        strRec = [strRec c];
    end
    %% 更新参数
    setappdata(handles.figure1, 'numRec', numRec); %更新已接收的数据个数
    setappdata(handles.figure1, 'strRec', strRec); %更新要显示的字符串
    
    global up_data;
    up_data=up_data+1;
    if(up_data==3)
        up_data=0;
    angle_updata(obj,event,handles);
    end
%     global pitch roll yaw bar mag_x mag_y mag_z GPS_longitude GPS_latitude GPS_altitude
%     axis(handles.axes6);
%     hold on;
%     plot3(handles.axes6,mag_x-150,mag_y+500,mag_z-100,'*','color',[mag_x/10000+0.5,mag_y/10000+0.5,mag_z/10000+0.5]);
%     view(45,45);
%     axis equal;
    
    
end

%% 更新姿态角数据
function angle_updata(obj,event,handles)
%% 提取参数
strRec=getappdata(handles.figure1, 'strRec');
try
str_search=strRec(end-800:end);
catch
    str_search=strRec;
end
line_T=strfind(str_search,'T');
if(size(line_T)<2)
    return;
end
last_line_T_num=max(size(line_T));
before_last_T_num=last_line_T_num-1;

last_line_T=line_T(last_line_T_num);
before_last_T=line_T(before_last_T_num);

str_last_parameter=str2num(str_search(before_last_T+1:last_line_T-1));
global pitch roll yaw bar mag_x mag_y mag_z GPS_longitude GPS_latitude GPS_altitude;
try
pitch=str_last_parameter(1);
roll=str_last_parameter(2);
yaw=str_last_parameter(3);
if ~isempty(str_last_parameter(4))
    try 
        if abs(bar(end)-str_last_parameter(4))<100
        bar=[bar str_last_parameter(4)];
        end
    catch
    end
end
if ~isempty(str_last_parameter(11))
    mag_x=str_last_parameter(11);
end
if ~isempty(str_last_parameter(12))
    mag_y=str_last_parameter(12);
end
if ~isempty(str_last_parameter(13))
    mag_z=str_last_parameter(13);
end
if ~isempty(str_last_parameter(14)) && ~isempty(str_last_parameter(15)) && ~isempty(str_last_parameter(16))
   if  str_last_parameter(14)>11600 && str_last_parameter(14)<11700 &&str_last_parameter(15)>3900 &&str_last_parameter(15)<4500 
    GPS_longitude=[GPS_longitude str_last_parameter(14)];
    GPS_latitude=[GPS_latitude str_last_parameter(15)];
    GPS_altitude=[GPS_altitude str_last_parameter(16)];
   end
end

%改变飞行姿态

Rz=[cos(yaw/180*pi) -sin(yaw/180*pi) 0;sin(yaw/180*pi) cos(yaw/180*pi) 0;0 0 1];
Ry=[cos(pitch/180*pi) 0 sin(pitch/180*pi);0 1 0;-sin(pitch/180*pi) 0 cos(pitch/180*pi)];
Rx=[1 0 0;0 cos(roll/180*pi) -sin(roll/180*pi);0 sin(roll/180*pi) cos(roll/180*pi)];

global fly_front fly_back fly_top fly_down mag_point;
front_point=Rz'*Ry'*Rx'*fly_front;
back_point=Rz'*Ry'*Rx'*fly_back;
top_point=Rz'*Ry'*Rx'*fly_top;
down_point=Rz'*Ry'*Rx'*fly_down;
 figure(3);

 hold off;
plot3(front_point(1,:),front_point(2,:),front_point(3,:),'.m');
hold on;
plot3(back_point(1,:),back_point(2,:),back_point(3,:),'.k');
plot3(top_point(1,:),top_point(2,:),top_point(3,:),'.r');
plot3(down_point(1,:),down_point(2,:),down_point(3,:),'.k');
axis equal;
view(90,0);
axis off;
catch
return;
end

%front_point_show=Rx*Ry*Rz*front_point;

%% 更新显示
    set(handles.pitch_angle,'string',pitch);
    set(handles.roll_angle,'string',roll);
    set(handles.yaw_angle,'string',yaw);
    set(handles.mag_x,'string',mag_x);
    set(handles.mag_y,'string',mag_y);
    set(handles.mag_z,'string',mag_z);
%     set(handles.GPS_longitude,'string',GPS_longitude(end));
%     set(handles.GPS_latitude,'string',GPS_latitude(end));
%     set(handles.GPS_altitude,'string',GPS_altitude(end));
    set(handles.bar_star_show,'string',bar(1));
    set(handles.bar_now_show,'string',bar(end));
    plot(handles.axes10,bar);
    
    try
        mag_point=[mag_point; mag_x mag_y mag_z];
        if mag_x<1000 && mag_x>-1000 && mag_y<1000 && mag_y>-1000 && mag_z<1000 && mag_z>-1000
            plot3(handles.axes7,mag_point(:,1),mag_point(:,2),mag_point(:,3),'*','color',[mag_x/2100+0.5 mag_y/2100+0.5 mag_z/2100+0.5]);
            axis equal;
        end
        mag_yaw=atan2(mag_y,mag_x)*180/pi;
        set(handles.mag_angle,'string',mag_yaw);
    catch
    end
    
    try
        plot3(handles.axes8,GPS_longitude, GPS_latitude, GPS_altitude,'r');
        plot(handles.axes9,GPS_longitude, GPS_latitude,'b');
    catch
    end

 %% 更新参数
    setappdata(handles.figure1, 'pitch', pitch); %更新俯仰角
    setappdata(handles.figure1, 'roll', roll); %更新滚转角
    setappdata(handles.figure1, 'yaw', yaw); %更新偏航角
    set(handles.receive_data, 'string',str_search(before_last_T+1:last_line_T-1));
   
 %   angle_updata(obj,event,handles);
% axes(handles.fly_model_show);
% axis on;

% % Update handles structure
% guidata(hObject, handles);    



function dataDisp(obj,event,handles)
% 串口的 TimerFcn 回调函数
% 串口数据显示
% 获取参数
%% 获取参数
hasData = getappdata(handles.figure1, 'hasData'); %串口是否收到数据
strRec = getappdata(handles.figure1, 'strRec');   %串口数据的字符串形式，定时显示该数据
numRec = getappdata(handles.figure1, 'numRec');   %串口接收到的数据个数
%% 若串口没有接收到数据，先尝试接收串口数据
if ~hasData
    ReceiveCallback(obj, event, handles);
end

%% 若串口有数据，显示串口数据
if hasData
    %% 给数据显示模块加互斥锁
    %% 在执行显示数据模块时，不接受串口数据，即不执行BytesAvailableFcn回调函数
    setappdata(handles.figure1, 'isShow', true); 
    %% 显示数据


    %% 更新hasData标志，表明串口数据已经显示
    setappdata(handles.figure1, 'hasData', false);
    %% 给数据显示模块解锁
    setappdata(handles.figure1, 'isShow', false);
end

% --- Executes on button press in Open_serial.
function Open_serial_Callback(hObject, eventdata, handles)
% hObject    handle to Open_serial (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
s=handles.serial_port;
set(s,'BytesAvailableFcnMode','terminator',...
	  'Terminator',{'T',13},...
	  'timeout',3,...
      'BytesAvailableFcn',{@ReceiveCallback,handles},...
      'TimerPeriod',0.05,...
      'Timerfcn',{@dataDisp,handles});
fclose(s);
try
fopen(s);
set(hObject,'enable','off');
set(handles.Close_serial,'enable','on');
disp('open success');
set(handles.text_data_save,'enable','off');
handles.serial_port=s;
guidata(hObject,handles);
catch
delete(instrfindall);
set(hObject,'enable','on');
set(handles.Close_serial,'enable','off');
disp('open failed');
errordlg('串口打开失败','错误信息');
end



% --- Executes during object creation, after setting all properties.
function Open_serial_CreateFcn(hObject, eventdata, handles)
% hObject    handle to Open_serial (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
set(hObject,'position',[16,4.3,12,1.7],...
            'fontsize',9);

% --- Executes on button press in Close_serial.
function Close_serial_Callback(hObject, eventdata, handles)
% hObject    handle to Close_serial (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
strRec = getappdata(handles.figure1, 'strRec');   %串口数据的字符串形式，定时显示该数据
s=handles.serial_port;
set(handles.Open_serial,'enable','on');
set(hObject,'enable','off');
fclose(s);
set(handles.text_data_save,'enable','on');
set(handles.receive_data,'string',strRec);
disp('serial_port closed');
clear s;

% --- Executes during object creation, after setting all properties.
function Close_serial_CreateFcn(hObject, eventdata, handles)
% hObject    handle to Close_serial (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
set(hObject,'position',[16,1.3,12,1.7],...
            'fontsize',9);

% --- Executes during object creation, after setting all properties.
function figure1_CreateFcn(hObject, eventdata, handles)
% hObject    handle to figure1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
set(hObject,'position',[50 100 180 50],...
          'Name','飞控串口助手',...
          'Numbertitle','off',...
          'menubar','none');


% --- Executes during object creation, after setting all properties.
function uipanel1_CreateFcn(hObject, eventdata, handles)
% hObject    handle to uipanel1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
set(hObject,'position',[5,40,30,8],...
            'title','串口设置',...
            'fontsize',9);


% --- Executes during object creation, after setting all properties.
function uipanel2_CreateFcn(hObject, eventdata, handles)
% hObject    handle to uipanel2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
set(hObject,'position',[40,40,25,8],...
            'title','姿态角',...
            'fontsize',9);



function pitch_angle_Callback(hObject, eventdata, handles)
% hObject    handle to pitch_angle (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of pitch_angle as text
%        str2double(get(hObject,'String')) returns contents of pitch_angle as a double

% --- Executes during object creation, after setting all properties.
function text3_CreateFcn(hObject, eventdata, handles)
% hObject    handle to text3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
set(hObject,'position',[1,4,8,2],...
            'string','pitch:',...
            'fontsize',9);


% --- Executes during object creation, after setting all properties.
function text4_CreateFcn(hObject, eventdata, handles)
% hObject    handle to text4 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
set(hObject,'position',[1,2,8,2],...
            'string','roll:',...
            'fontsize',9);


% --- Executes during object creation, after setting all properties.
function text5_CreateFcn(hObject, eventdata, handles)
% hObject    handle to text5 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
set(hObject,'position',[1,0,8,2],...
            'string','yaw:',...
            'fontsize',9);



% --- Executes during object creation, after setting all properties.
function pitch_angle_CreateFcn(hObject, eventdata, handles)
% hObject    handle to pitch_angle (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
set(hObject,'position',[12,4.5,10,1.5],...
            'string','0.0',...
            'fontsize',9);
% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

% --- Executes during object creation, after setting all properties.
function roll_angle_CreateFcn(hObject, eventdata, handles)
% hObject    handle to roll_angle (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
set(hObject,'position',[12,2.5,10,1.5],...
            'string','0.0',...
            'fontsize',9);
% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes during object creation, after setting all properties.
function yaw_angle_CreateFcn(hObject, eventdata, handles)
% hObject    handle to yaw_angle (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
set(hObject,'position',[12,0.5,10,1.5],...
            'string','0.0',...
            'fontsize',9);
% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


function edit2_Callback(hObject, eventdata, handles)
% hObject    handle to edit2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit2 as text
%        str2double(get(hObject,'String')) returns contents of edit2 as a double


% --- Executes during object creation, after setting all properties.
function edit2_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function roll_angle_Callback(hObject, eventdata, handles)
% hObject    handle to roll_angle (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of roll_angle as text
%        str2double(get(hObject,'String')) returns contents of roll_angle as a double





function edit4_Callback(hObject, eventdata, handles)
% hObject    handle to edit4 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit4 as text
%        str2double(get(hObject,'String')) returns contents of edit4 as a double


% --- Executes during object creation, after setting all properties.
function edit4_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit4 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function yaw_angle_Callback(hObject, eventdata, handles)
% hObject    handle to yaw_angle (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of yaw_angle as text
%        str2double(get(hObject,'String')) returns contents of yaw_angle as a double



% --- Executes during object creation, after setting all properties.
function uipanel3_CreateFcn(hObject, eventdata, handles)
% hObject    handle to uipanel3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
set(hObject,'position',[70,32,30,16],...
            'title','地磁数据',...
            'fontsize',9);

% --- Executes during object creation, after setting all properties.
function fly_model_show_CreateFcn(hObject, eventdata, handles)
% hObject    handle to fly_model_show (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: place code in OpeningFcn to populate fly_model_show

        
% --- Executes during object creation, after setting all properties.
function flying_attitude_CreateFcn(hObject, eventdata, handles)
% hObject    handle to flying_attitude (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
% Hint: place code in OpeningFcn to populate flying_attitude
set(hObject,'box','on');
axes(hObject);
x=1:0.1:pi*4;
y=sin(x);
plot(x,y);
axis off;


% --- Executes during object creation, after setting all properties.
function uipanel5_CreateFcn(hObject, eventdata, handles)
% hObject    handle to uipanel5 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
set(hObject,'position',[102,32,30,16],...
            'title','GPS信息',...
            'fontsize',9);


% --- Executes during object creation, after setting all properties.
function uipanel6_CreateFcn(hObject, eventdata, handles)
% hObject    handle to uipanel6 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
set(hObject,'position',[5,19,60,21],...
            'title','接收数据',...
            'fontsize',9);

% --- Executes during object creation, after setting all properties.
function receive_data_CreateFcn(hObject, eventdata, handles)
% hObject    handle to receive_data (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
set(hObject,'position',[4,6,51,13],...
            'string','',...
            'fontsize',9,...
            'Max',10000);
global data_now;
data_now='';
% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

% --- Executes during object creation, after setting all properties.
function text_data_clean_CreateFcn(hObject, eventdata, handles)
% hObject    handle to text_data_clean (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
set(hObject,'position',[4,3,20,2],...
            'string','清空数据',...
            'fontsize',9);

% --- Executes during object creation, after setting all properties.
function text_data_save_CreateFcn(hObject, eventdata, handles)
% hObject    handle to text_data_save (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
set(hObject,'position',[35,3,20,2],...
            'string','保存数据',...
            'fontsize',9);
        
        
% --- Executes during object creation, after setting all properties.
function save_path_CreateFcn(hObject, eventdata, handles)
% hObject    handle to save_path (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
set(hObject,'position',[4,0.5,51,1.5],...
            'string','E:\研究生\05自己毕业\13地面站\save_data.txt',...
            'fontsize',9);
% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



% --- Executes during object creation, after setting all properties.
function edit7_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit7 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

% --- Executes during object creation, after setting all properties.
function receive_data_slider_CreateFcn(hObject, eventdata, handles)
% hObject    handle to receive_data_slider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
set(hObject,'position',[52,1,3,13],...
            'string','',...
            'fontsize',9,...
            'visible','off',...
            'max',1000,...
            'HorizontalAlignment','left');

% Hint: slider controls usually have a light gray background.
if isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor',[.9 .9 .9]);
end

% --- Executes during object creation, after setting all properties.
function uipanel7_CreateFcn(hObject, eventdata, handles)
% hObject    handle to uipanel7 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
set(hObject,'position',[70,3,75,28],...
            'title','地图',...
            'fontsize',9);



function receive_data_Callback(hObject, eventdata, handles)
% hObject    handle to receive_data (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

%set(handles.receive_data_slider,'value',str2num(get(hObject,'string')));

% Hints: get(hObject,'String') returns contents of receive_data as text
%        str2double(get(hObject,'String')) returns contents of receive_data as a double


% --- Executes on slider movement.
function receive_data_slider_Callback(hObject, eventdata, handles)
% hObject    handle to receive_data_slider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(handles.receive_data,'string',num2str(get(hObject,'value')));
% Hints: get(hObject,'Value') returns position of slider
%        get(hObject,'Min') and get(hObject,'Max') to determine range of slider


% --- Executes on button press in text_data_clean.
function text_data_clean_Callback(hObject, eventdata, handles)
% hObject    handle to text_data_clean (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(handles.receive_data,'string','');
%% 清空要显示的字符串
setappdata(handles.figure1, 'strRec', '');

% --- Executes on button press in text_data_save.
function text_data_save_Callback(hObject, eventdata, handles)
save_text=get(handles.receive_data,'string');
save_path=get(handles.save_path,'string');
string_text=size(save_text);

fid=fopen(save_path,'w');
for i=1:1:string_text(1)
fprintf(fid,'%s',save_text(i,:)');
fprintf(fid,'\r\n');
end
fclose(fid);
disp('save success');
% hObject    handle to text_data_save (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)



function edit7_Callback(hObject, eventdata, handles)
% hObject    handle to edit7 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit7 as text
%        str2double(get(hObject,'String')) returns contents of edit7 as a double



function save_path_Callback(hObject, eventdata, handles)
% hObject    handle to save_path (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of save_path as text
%        str2double(get(hObject,'String')) returns contents of save_path as a double

% --- Executes on button press in pushbutton6.
function pushbutton6_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton6 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

function mag_x_Callback(hObject, eventdata, handles)
% hObject    handle to mag_x (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of mag_x as text
%        str2double(get(hObject,'String')) returns contents of mag_x as a double


% --- Executes during object creation, after setting all properties.
function mag_x_CreateFcn(hObject, eventdata, handles)
% hObject    handle to mag_x (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
set(hObject,'position',[4,13,10,1]);
% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function mag_y_Callback(hObject, eventdata, handles)
% hObject    handle to mag_y (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of mag_y as text
%        str2double(get(hObject,'String')) returns contents of mag_y as a double


% --- Executes during object creation, after setting all properties.
function mag_y_CreateFcn(hObject, eventdata, handles)
% hObject    handle to mag_y (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
set(hObject,'position',[4,11.5,10,1]);
% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function mag_z_Callback(hObject, eventdata, handles)
% hObject    handle to mag_z (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of mag_z as text
%        str2double(get(hObject,'String')) returns contents of mag_z as a double


% --- Executes during object creation, after setting all properties.
function mag_z_CreateFcn(hObject, eventdata, handles)
% hObject    handle to mag_z (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
set(hObject,'position',[4,10,10,1]);
% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function mag_angle_Callback(hObject, eventdata, handles)
% hObject    handle to mag_angle (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of mag_angle as text
%        str2double(get(hObject,'String')) returns contents of mag_angle as a double


% --- Executes during object creation, after setting all properties.
function mag_angle_CreateFcn(hObject, eventdata, handles)
% hObject    handle to mag_angle (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
set(hObject,'position',[18,11.5,10,1]);
% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes during object creation, after setting all properties.
function axes7_CreateFcn(hObject, eventdata, handles)
% hObject    handle to axes7 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
set(hObject,'position',[7,1.6,20,8]);
% plot3(0,0,0,'*r');
% hold on;
% Hint: place code in OpeningFcn to populate axes7


% --- Executes during object creation, after setting all properties.
function axes8_CreateFcn(hObject, eventdata, handles)
% hObject    handle to axes8 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
set(hObject,'position',[2,0.5,25,9]);
% Hint: place code in OpeningFcn to populate axes8



function GPS_longitude_Callback(hObject, eventdata, handles)
% hObject    handle to GPS_longitude (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of GPS_longitude as text
%        str2double(get(hObject,'String')) returns contents of GPS_longitude as a double


% --- Executes during object creation, after setting all properties.
function GPS_longitude_CreateFcn(hObject, eventdata, handles)
% hObject    handle to GPS_longitude (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
set(hObject,'position',[15,13,10,1]);
% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function GPS_latitude_Callback(hObject, eventdata, handles)
% hObject    handle to GPS_latitude (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of GPS_latitude as text
%        str2double(get(hObject,'String')) returns contents of GPS_latitude as a double


% --- Executes during object creation, after setting all properties.
function GPS_latitude_CreateFcn(hObject, eventdata, handles)
% hObject    handle to GPS_latitude (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
set(hObject,'position',[15,11.5,10,1]);
% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function GPS_altitude_Callback(hObject, eventdata, handles)
% hObject    handle to GPS_altitude (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of GPS_altitude as text
%        str2double(get(hObject,'String')) returns contents of GPS_altitude as a double


% --- Executes during object creation, after setting all properties.
function GPS_altitude_CreateFcn(hObject, eventdata, handles)
% hObject    handle to GPS_altitude (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
set(hObject,'position',[15,10,10,1]);
% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes during object creation, after setting all properties.
function x_CreateFcn(hObject, eventdata, handles)
% hObject    handle to x (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
set(hObject,'position',[1,13,2,1]);


% --- Executes during object creation, after setting all properties.
function text8_CreateFcn(hObject, eventdata, handles)
% hObject    handle to text8 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
set(hObject,'position',[1,10,2,1]);


% --- Executes during object creation, after setting all properties.
function text7_CreateFcn(hObject, eventdata, handles)
% hObject    handle to text7 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
set(hObject,'position',[1,11.5,2,1]);


% --- Executes during object creation, after setting all properties.
function axes9_CreateFcn(hObject, eventdata, handles)
% hObject    handle to axes9 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
set(hObject,'position',[2,1,70,25]);

% Hint: place code in OpeningFcn to populate axes9


% --- Executes during object creation, after setting all properties.
function text9_CreateFcn(hObject, eventdata, handles)
% hObject    handle to text9 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
set(hObject,'position',[1,13,13,1]);


% --- Executes during object creation, after setting all properties.
function text10_CreateFcn(hObject, eventdata, handles)
% hObject    handle to text10 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
set(hObject,'position',[1,11.5,13,1]);


% --- Executes during object creation, after setting all properties.
function text11_CreateFcn(hObject, eventdata, handles)
% hObject    handle to text11 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
set(hObject,'position',[1,10,13,1]);


% --- Executes during object creation, after setting all properties.
function uipanel8_CreateFcn(hObject, eventdata, handles)
% hObject    handle to uipanel8 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
set(hObject,'position',[134,32,30,16],...
            'title','气压计',...
            'fontsize',9);


% --- Executes during object creation, after setting all properties.
function bar_star_CreateFcn(hObject, eventdata, handles)
% hObject    handle to bar_star (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
set(hObject,'position',[1,13,13,1]);



function bar_star_show_Callback(hObject, eventdata, handles)
% hObject    handle to bar_star_show (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of bar_star_show as text
%        str2double(get(hObject,'String')) returns contents of bar_star_show as a double


% --- Executes during object creation, after setting all properties.
function bar_star_show_CreateFcn(hObject, eventdata, handles)
% hObject    handle to bar_star_show (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
set(hObject,'position',[15,13,10,1],...
            'string','0.0');
% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function bar_now_show_Callback(hObject, eventdata, handles)
% hObject    handle to bar_now_show (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of bar_now_show as text
%        str2double(get(hObject,'String')) returns contents of bar_now_show as a double


% --- Executes during object creation, after setting all properties.
function bar_now_show_CreateFcn(hObject, eventdata, handles)
% hObject    handle to bar_now_show (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
set(hObject,'position',[15,11.5,10,1]);
% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes during object creation, after setting all properties.
function bar_now_CreateFcn(hObject, eventdata, handles)
% hObject    handle to bar_now (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
set(hObject,'position',[1,11.5,13,1]);


% --- Executes during object creation, after setting all properties.
function axes10_CreateFcn(hObject, eventdata, handles)
% hObject    handle to axes10 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
set(hObject,'position',[2,0.5,25,9]);
% Hint: place code in OpeningFcn to populate axes10
