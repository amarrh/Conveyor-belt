

function varargout = pokretnaTraka(varargin)
% POKRETNATRAKA MATLAB code for pokretnaTraka.fig
%      POKRETNATRAKA, by itself, creates a new POKRETNATRAKA or raises the existing
%      singleton*.
%
%      H = POKRETNATRAKA returns the handle to a new POKRETNATRAKA or the handle to
%      the existing singleton*.
%
%      POKRETNATRAKA('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in POKRETNATRAKA.M with the given input arguments.
%
%      POKRETNATRAKA('Property','Value',...) creates a new POKRETNATRAKA or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before pokretnaTraka_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to pokretnaTraka_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help pokretnaTraka

% Last Modified by GUIDE v2.5 26-Oct-2018 21:07:23

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @pokretnaTraka_OpeningFcn, ...
                   'gui_OutputFcn',  @pokretnaTraka_OutputFcn, ...
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


% --- Executes just before pokretnaTraka is made visible.
function pokretnaTraka_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to pokretnaTraka (see VARARGIN)

% Choose default command line output for pokretnaTraka
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes pokretnaTraka wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = pokretnaTraka_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on button press in start.
function start_Callback(hObject, eventdata, handles)
% hObject    handle to start (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global stop;
stop = false;
remPort=5000;      
host='192.168.1.11';  
locPort=9055;
u = udp(host,'RemotePort',remPort,'LocalPort',locPort); 
fopen(u);
fwrite(u, 'Pokreni');
A = fread(u,256,'char');
fclose(u);
delete(u)
clear u 
if char(A) == 'p'
    set(handles.text1, 'String', 'Traka je aktivirana!');
end


pause(1);

var = true;

while(var == true && stop == false)
    u = udp(host,'RemotePort',remPort,'LocalPort',locPort); 
    fopen(u);
    A = fread(u,256,'char');
    fclose(u);
    delete(u)
    clear u 
    if char(A) == 'k'
        set(handles.text1, 'String', 'Zastoj! Provjeriti traku!');
        A = 'a';
        var = false;
    end
    pause(1);
end
% --- Executes on button press in stop.
function stop_Callback(hObject, eventdata, handles)
% hObject    handle to stop (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global stop;
stop = true;
remPort=5000;      
host='192.168.1.11';  
locPort=9055;
u = udp(host,'RemotePort',remPort,'LocalPort',locPort); 
fopen(u);
fwrite(u, 'Zaustavi');
A=fread(u,256,'char');
fclose(u);
delete(u)
clear u

if char(A) == 'z'
    set(handles.text1, 'String', 'Traka je zaustavljena!');
end

