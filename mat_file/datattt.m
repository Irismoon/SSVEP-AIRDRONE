data_index=data_index+1;
data_index_mod=mod(data_index-1,DATA_LENGTH)+1;
%�����˲�
data_unfilter(data_index_mod,:)=x(ch);%C++���ӽ��������ݴ�����
if data_index_mod<=filt_n+1
    xx=[filter_b(2:data_index_mod),-filter_a(2:data_index_mod)];
    yy=[data_unfilter(data_index_mod-1:-1:1,:);data(data_index-1:-1:1,:)];
else
    xx=[filter_b(2:filt_n+1),-filter_a(2:filt_n+1)];
    yy=[data_unfilter(data_index_mod-1:-1:data_index_mod-filt_n,:);
        data(data_index_mod-1:-1:data_index_mod-filt_n,:)];
end
data(data_index_mod,:)=filter_b(1)*data_unfilter(data_index_mod,:);
if(size(xx,2)>0)
    data(data_index_mod,:)=data(data_index_mod,:)+xx*yy;
end

%���ݴ���
if mod(data_index,slide)==0%ÿslide�����ݴ���һ��
    if IS_TRAIN%train
        if mod(data_index,TRIAL)>TRIAL_TRAISIENT || mod(data_index,TRIAL)==0
            index=index+1;
            for i=1:ch_size
                Y=data(data_index_mod-segment+1:data_index_mod,i);%һ��ͨ������������
                for j = 1:frecount
                    fea_train(index,(i-1)*frecount+j)=norm(tri{j}*Y);
                end
            end%��ȡ����
            for i = 1:frecount
                fea_tran(index,i) = fea_train(index,i)+fea_train(index,2*frecount+i)+fea_train(index,3*frecount+i)+fea_train(index,frecount+i);%��i��i/2��Ƶ�ʵĻ���
            end
            trigger(index)=x(DATA_CHANNEL+1);%x�����һ���Ǳ�ǩ
            %disp([data_index, trigger(index)])
            if data_index==DATA_LENGTH
                N=length(trigger);
                [C,err,P,logp,coeff] = classify(fea_tran(1,:),fea_tran(1:N,:),trigger(1:N));
                save('coeff.mat','coeff');%��ͬ���֮��ı߽����ߵĲ���
            end%�����һ������ִ�з���
        end  
        signal=-1;
    else%play
        if data_index_mod>segment%
            for i=1:ch_size
                Y=data(data_index_mod-segment+1:data_index_mod,i);
                for j = 1:2*frecount
                    fea(1,(i-1)*2*frecount+j)=norm(tri{j}*Y);
                end
                
            end
            for i = 1:2*frecount
                feaa(1,i) = fea(1,i)+fea(1,2*frecount+i)+fea(1,3*frecount+i)+fea(1,frecount+i);%��i��i/2��Ƶ�ʵĻ���/г��
            end
            r=zeros(1,4);
            r(1,1)=r(1,1)+(coeff(1,2).linear'*fea'+coeff(1,2).const>0);%x*L+K>0���߽�������һ�κ���
            r(1,1)=r(1,1)+(coeff(1,3).linear'*fea'+coeff(1,3).const>0);
            r(1,1)=r(1,1)+(coeff(1,4).linear'*fea'+coeff(1,4).const>0);

            r(1,2)=r(1,2)+(coeff(2,1).linear'*fea'+coeff(2,1).const>0);
            r(1,2)=r(1,2)+(coeff(2,3).linear'*fea'+coeff(2,3).const>0);
            r(1,2)=r(1,2)+(coeff(2,4).linear'*fea'+coeff(2,4).const>0);

            r(1,3)=r(1,3)+(coeff(3,1).linear'*fea'+coeff(3,1).const>0);
            r(1,3)=r(1,3)+(coeff(3,2).linear'*fea'+coeff(3,2).const>0);
            r(1,3)=r(1,3)+(coeff(3,4).linear'*fea'+coeff(3,4).const>0);
            
            r(1,4)=r(1,4)+(coeff(4,1).linear'*fea'+coeff(4,1).const>0);
            r(1,4)=r(1,4)+(coeff(4,2).linear'*fea'+coeff(4,2).const>0);
            r(1,4)=r(1,4)+(coeff(4,3).linear'*fea'+coeff(4,3).const>0);

            [b,signal]=max(r(1,1:4));
            signal=signal-1%���C++���ԣ������������0��1��2��3��
            Signal= [Signal signal];
        end
    end
else
    signal=-1;
end


