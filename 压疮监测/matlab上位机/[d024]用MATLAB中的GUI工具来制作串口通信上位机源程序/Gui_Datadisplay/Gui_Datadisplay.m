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

% Last Modified by GUIDE v2.5 05-Nov-2020 10:27:26

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
%% �ı䴰�����Ͻǵ�ͼ��Ϊicon.jpg
javaFrame = get(hObject,'JavaFrame');
javaFrame.setFigureIcon(javax.swing.ImageIcon('icon.jpg'));
%% �˿ںͲ����ʳ�ʼ��
global COM;      %ҳ���ʼ��
global rate;
global s;

COM='COM7';
s=serial(COM);
rate = 115200;


set(handles.ppCOM,'value', 7);
set(handles.ppBaudRates,'value',1);
set(handles.pbCloseSerial,'Enable','off');%��ʼ���رհ�ť�ò��ˣ���ʹ��
set(handles.begin_get,'UserData',0);
setappdata(handles.save,'ApplicationData',0);

% Choose default command line output for Gui_Datadisplay
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);%�������õľ��

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

% --- Executes during object creation, after setting all properties.
function Value_CreateFcn(hObject, eventdata, handles)
% hObject    handle to Value (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
input = str2num(get(hObject,'String'));
if (isempty(input))
set(hObject,'String','0')
end
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
global rate; %����ȫ�ֱ���rate�����ڴ���ͨ������
global COM;  %����ȫ�ֱ���COM������ѡ��ͬ�Ĵ���
global out;  %����ȫ�ֱ���out�������жϴ����Ƿ�� out=1���ڴ򿪣�out=0���ڹر�
global datas; %����ȫ�ֱ���datas,���ڴ洢���ڷ�����������
out=1;        %���ڴ�
if (isempty(COM) && get(handles.ppCOM,'Value')==1 )           % ����˿�1���ڣ��洴���˿�  
    s=serial(COM);
    set(s,'BaudRate',rate);%%%Baud��ʼ��
    set(s,'DataBits',8);%%%�������ݳ���
    set(s,'StopBits',1);%%%����ֹͣλ����
    set(s,'timeout',10);%%���ý���ʱ��Ϊ20s,Ĭ����10s
    % set(s,'Terminator',10);%%���ûس�����Ϊ��ֹ����Ŀ��Ϊ�˴�����յ�������
    set(s,'InputBufferSize',1024000);%%%�������뻺������СΪ1M


    % �����¼��ص�����
    s.BytesAvailableFcnMode='byte';
    s.BytesAvailableFcnCount=6; %���뻺��������10���ֽڴ����ص�����
    s.BytesAvailableFcn={@EveBytesAvailableFcn,handles};%�ص�������ָ��

    fopen(s);%�򿪴���
    get(s);
    set(handles.begin_get,'Enable','On');
    set(handles.pbCloseSerial,'Enable','On');
    set(handles.pbOpenSerial,'Enable','Off');
    set(handles.exit_window,'Enable','Off');
else
     msgbox('���ڲ����ڻ�ռ�ã�');
    % ������ֱ�Ӵ�����յ�������
% k=1;
% datas=zeros([1,100]);%��ȫ�ֱ���datas����ʼֵΪ1*100�������
% while(out==1)
% if s.BytesAvailable %��ѯ�Ƿ���յ�����
%     datas(1,k)=fread(s,n);%�Զ����ƶ�ȡ����s�е����ݲ��洢��datas��
% end
% plot(handles.axes1,datas);
% % fprintf('���յ�������\n');
% 
% d=get(handles.Value,'String');%��ȡValue��̬�洢���������ֵ����Ŀ��ֵ
% y=str2num(d);%���ַ���dת��Ϊ�����ͣ����洢��y��
% e=size(datas);%����eΪ��datas��ͬ��ʽ������
% i=1;
% while(i<=k) %��e���鸳ֵȫ��Ϊy,��ΪĿ��ֵ
% e(1,i)=y;
% i=i+1;
% end
% hold on %���ֽ��յ������ݻ��Ƶ�ͼ��������Ŀ��ֵ��ͼ��
% plot(handles.axes1,e,'--')%��ͼ��gui�����е�axes1�е�ͼ
% hold off
% c = num2str(datas(1,k)); %��ʵʱ���ݴ洢��c��
% set(handles.Datas,'String',c);%�ھ�̬�洢��Datas����ʾ����ʵʱ����
% guidata(hObject, handles); % ���½ṹ��
% k=k+1;
% end
fprintf('ceshi_dakaichuankou\n');%�����в����Ƿ���˴���

% �ص�����
function EveBytesAvailableFcn( t,event,handles )  
global s;  
global a;  
global d;  
global arrX;  
global count;
global n;
if(getappdata(handles.begin_get,'ApplicationData')~=0)    % �յ������ɼ��źŵ���Ϣ

   
arrX=zeros([1,100]);  
% arrY=[1,1000];  
% arrZ=[1,1000];  
fprintf('Receive callback');  
n=s.BytesAvailable;
if n
a=fread(s,n);%ȡ�������е����� 

end

% a=fread(s);
% M=textscan(a,'%s%.2f%s','delimiter',' ');%��ͬ��������֮���ÿո�ָ����ֿ�  
% data = [data , '\n'];  
% [~,x,~] = deal(M{:});  
% arrX(1,count) = x;  %ȡ���Ӵ��ڵõ��ĸ���������
% arrY(1,count) = y;  
% arrZ(1,count) = z;

% a = strcat(a,char(10),get(handles.editR,'string'));  %char(10)��ʾ�س�����
a=char(a');%���Ӵ����豸�ж�ȡ����ASCii��ת��Ϊ�ַ���������ʾ��a'��������ʾ
data1=str2num(a); %��ͨ��1�����ֱ�����data1��
a = strcat('��ѹΪ��',a,'V');   %�����������ʾ���ĸ�ʽ����Ϊ����ת��asci��ת������ʾ
set(handles.editR,'string',a);
fprintf('����\r\n');
set(handles.text4,'string',data1);%��text4����ʾ���յ��ĵ�ѹֵ  
% plot(handles.axes1,arrX);%�ڿؼ��е�����ͼ����ʾ���յ��ĵ�ѹֵ  
% hold on;  
% count=count+1;  
% if count==100  
%     count=1;  
%     clf;  
% end

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
        rate=19200;
    case 3
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

fprintf('Close_COM');
end


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
set(handles.Datas,'string','');


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


% --- Executes during object creation, after setting all properties.
function Datas_CreateFcn(hObject, eventdata, handles)
% hObject    handle to Datas (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called


% --- Executes on button press in savedata.
function savedata_Callback(hObject, eventdata, handles)
% hObject    handle to savedata (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global datas; %����ȫ�ֱ���datas
xlswrite('data.xlsx',datas);




% --- Executes on button press in go_on.
function go_on_Callback(hObject, eventdata, handles)
% hObject    handle to go_on (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
uiresume; %������ʾ


% --- Executes on button press in openDTR.
function openDTR_Callback(hObject, eventdata, handles)
% hObject    handle to openDTR (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of openDTR
global s;
s.DataTerminalReady='off';%%����DTRΪ����������Ϊ


% --- Executes on button press in excit_window.
function excit_window_Callback(hObject, eventdata, handles)
% hObject    handle to excit_window (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
close(gcbf);


% --- Executes during object creation, after setting all properties.
function axes3_CreateFcn(hObject, eventdata, handles)
% hObject    handle to axes3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: place code in OpeningFcn to populate axes3



function save_path_show_Callback(hObject, eventdata, handles)
% hObject    handle to save_path_show (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of save_path_show as text
%        str2double(get(hObject,'String')) returns contents of save_path_show as a double


% --- Executes during object creation, after setting all properties.
function save_path_show_CreateFcn(hObject, eventdata, handles)
% hObject    handle to save_path_show (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in save_path.
function save_path_Callback(hObject, eventdata, handles)
% hObject    handle to save_path (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(handles.save,'Enable','On');
[file, pathname, index] = uiputfile({'*.txt';'*.dat';'*.mat'},'�ļ����Ϊ');
file1 = strcat(pathname,file);
set(handles.save_play,'String',file1);
set(handles.save_lujin,'UserData',index);      % �ж��Ƿ��趨�˱����ļ���·��


% --- Executes on button press in save.
function save_Callback(hObject, eventdata, handles)
% hObject    handle to save (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on button press in begin_get_data.
function begin_get_data_Callback(hObject, eventdata, handles)
% hObject    handle to begin_get_data (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% �����źţ����ݸ��жϣ����ź��ݴ��ڲɼ��ؼ���

% t = str2num(get(handles.save_time,'String'));
index = get(handles.save_path,'UserData');         % �Ƿ��趨�洢·��
global s;
[a,b] = fread(s);
if ( b==0 )
    msgbox('��ǰ���������룡');
    set(handles.save,'Enable','Off');
else
    if(get(handles.begin_get,'UserData')==0)    %UserData������ʾ��ť�п�ʼ�ɼ���ֹͣ�ɼ��л�
        set(handles.begin_get,'String','ֹͣ�ɼ�');
        set(handles.begin_get,'BackgroundColor',[1 0 0]);
        setappdata(handles.begin_get,'ApplicationData',1);   % ���ڴ��ݸ��ж���Ӧ���������Խ��ղ���ʾ������
        set(handles.begin_get,'UserData',1);
        if(  index~=0 )                                       % �趨�洢����
            set(handles.save,'Enable','On');
        else
            set(handles.save,'Enable','Off');
        end
    else
        set(handles.begin_get,'String','��ʼ�ɼ�');
        setappdata(handles.begin_get,'ApplicationData',0); 
        set(handles.begin_get,'BackgroundColor',[0.8 0.8 0.8]);
        set(handles.begin_get,'UserData',0);
        set(handles.save,'Enable','Off');
    end
    
end




function current_data_Callback(hObject, eventdata, handles)
% hObject    handle to current_data (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of current_data as text
%        str2double(get(hObject,'String')) returns contents of current_data as a double


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
