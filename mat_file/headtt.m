clc;clear all;
IS_TRAIN=1;
Fs=128;
filt_n=4;
[filter_b,filter_a]=butter(filt_n,2/(Fs/2),'high');%4�װ�����˹��ͨ�˲���


DATA_LENGTH=160*Fs;
Signal = [];
DATA_CHANNEL=8;
ch=[1,3,5,7];%ָǰ��AF3,AF4,F3,F4,����Emotiv���Ŵ���������Ҷ����P3,P4,O1,O2����
ch_size=length(ch);
data_index=0;
index=0;
data_unfilter=zeros(DATA_LENGTH,ch_size);
data=zeros(DATA_LENGTH,ch_size);

freqnum = 5;
freq1 = linspace(12.4,12.6,freqnum);
freq2 = linspace(9.3,9.5,freqnum);
freq3 = linspace(8.2,8.4,freqnum);
freq4 = linspace(7.4,7.6,freqnum);

%freq = [12.525,9.3439,8.2973,7.5125];%LBH
%freq = [12.525,9.4934,8.2973,15.0249];%LSZ
%freq = [12.5 9.37 8.33 6.8182];
%freq=[12 10 8.5714 6.667];
frecount = 4;%length(freq);
%peroid=1000./freq;
%freq=[12 8.5714 6.6667]
%peroid=[80,120,200];
segment=2*Fs;%����
slide=Fs/8;%��������
TRIAL=Fs*5;%ѵ��ʱ��ÿ��ͬ���ǩ�ĳ���
TRIAL_TRAISIENT=Fs/4+segment;
TRIAL_STEADY=TRIAL-TRIAL_TRAISIENT;%ÿ��trial��25%����������̬����

fea_train=zeros(floor(DATA_LENGTH*(TRIAL_STEADY/TRIAL)/slide),2*frecount*ch_size);
fea_tran=zeros(floor(DATA_LENGTH*(TRIAL_STEADY/TRIAL)/slide),2*frecount);
fea=zeros(1,2*frecount*ch_size);
feaa = zeros(1,2*frecount);
trigger=zeros(DATA_LENGTH*(TRIAL_STEADY/TRIAL)/slide,1);
tri1=cell(2*freqnum,1);
tri2=cell(2*freqnum,1);
tri3=cell(2*freqnum,1);
tri4=cell(2*freqnum,1);
for i=1:freqnum
    tri1{i} = [cos(2*pi*freq1(i)/Fs*(1:segment));-sin(2*pi*freq1(i)/Fs*(1:segment))];
    tri1{i+freqnum}=[cos(2*pi*2*freq1(i)/Fs*(1:segment));-sin(2*pi*2*freq1(i)/Fs*(1:segment))];
    tri2{i} = [cos(2*pi*freq2(i)/Fs*(1:segment));-sin(2*pi*freq2(i)/Fs*(1:segment))];
    tri2{i+freqnum}=[cos(2*pi*2*freq2(i)/Fs*(1:segment));-sin(2*pi*2*freq2(i)/Fs*(1:segment))];
    tri3{i} = [cos(2*pi*freq3(i)/Fs*(1:segment));-sin(2*pi*freq3(i)/Fs*(1:segment))];
    tri3{i+freqnum}=[cos(2*pi*2*freq3(i)/Fs*(1:segment));-sin(2*pi*2*freq3(i)/Fs*(1:segment))];
    tri4{i} = [cos(2*pi*freq4(i)/Fs*(1:segment));-sin(2*pi*freq4(i)/Fs*(1:segment))];
    tri4{i+freqnum}=[cos(2*pi*2*freq4(i)/Fs*(1:segment));-sin(2*pi*2*freq4(i)/Fs*(1:segment))];
end 
signal=-1;