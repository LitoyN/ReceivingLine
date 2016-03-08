answers=zeros(1,10);

for i=1:5
    fprintf(serial1,'200');
    pause(.5);
    temp=fscanf(serial1);
    answers(i) = str2num(temp);
    pause(.25);
end