%Mean-Shift Proslediawane
%Matlab 7.9.0 (R2009b)
clc
clear
%%
%%Inicializirane na parametrite na kamerata
vid = videoinput('winvideo', 1,'YUY2_320x240');
set(vid,'ReturnedColorSpace','rgb','TriggerRepeat',1000,'FramesPerTrigger',1);
vid_src=getselectedsource(vid);
set(vid_src,'BacklightCompensation','off','ExposureMode','manual','WhiteBalanceMode','auto');%,'WhiteBalance',6500);
triggerconfig(vid,'manual');
start(vid);
%Suzdawane na AVI obekt (fail) w koito shte budat zapiswani kadri
%aviobj = avifile('Mean-Shift_Tracking3.avi');
%%
%Inicializirane na nachalnite koordinati na obekta (y0,x0) i poluchenite nowi
%koordinati (y1,x1)
flag=0;
y0=0;
x0=0;
y1=0;
x1=0;
%%

for step=1:1:inf
   trigger(vid);
   
   %Poluchawane na kadur ot kamerata
   Frame=getdata(vid,1);
   %Frame=ImageNormalisation(Frame);
   if(flag==1)
       %% Poluchavane na histogramata na kandidat-celta i izchisliawane na wektora na otmestwaneto
       CandidateFrame=Frame;
       % purwo se wzima parche ot izobrajenieto sus sushtite koordinati kato shablona
       CandidateTarget=imcrop(CandidateFrame,Newcoords);
       %preobrazuwa se w iarkostno
       GrayCandidateTarget=rgb2gray(CandidateTarget);
       %preobrazuwa se w "double" tip danni za da e suwmestimo s EpanechnikovMask 
       GrayCandidateTarget=double(GrayCandidateTarget);
       %izchisliawane na preteglenite stoinosti na piskelite ot parcheto
       WeightedCandidateTarget=GrayCandidateTarget.*EpanechnikovMask;
       %niama otricatelni stoinosti zashoto i GrayCandidateTarget i 
       %EpanechnikovMask imat samo polojitelni koeficienti. Preobrazuwa se
       %w uint8 za da se zakrugliat stoinostite do celi chisla i da moje da
       %se izchisli po-prosta histograma
       WeightedCandidateTarget=uint8(WeightedCandidateTarget);
       %izchisliawane na histogramata na kandidat-celta
       [CandidateTargetCounts,CandidateTargetCountsX]=imhist(WeightedCandidateTarget); %Candidate Target
       %izchisliawane na X i Y parametrite na Mean-Shift vektora izpolzwaiki dwete histogrami-na shablona i na kandidat-celta 
       [MhY MhX]=MeanShiftVectEstimate(WeightedCandidateTarget,TargetCounts,TargetCountsX,CandidateTargetCounts,CandidateTargetCountsX);
       %Preobrazuwane na wektora ot dekartowi koordinati w ramkite na
       %parcheto w globalni matrichni koordinati w ramkite na kadura.
       %Nikoia
       %ot koordinatite ne e normalizirana. Nawsiakude w koda se zapazwat
       %standartnite dekartowi koordinati [0;+-MaxSize]
       y1=round(y0-MhY);
       x1=round(x0+MhX);
       %Zadawane na nowite stoinosti na y0 i x0 i posledwashto
       %preizchisliawane w cikul na nowite stoinosti za y1 i x1
       y0=y1;
       x0=x1;
       %Postawiane na marker wurhu namerenie obekt
       TrackedObject=draw_rectangle_marker(CandidateFrame,MaxTemplateSize,y1,x1,0,255,0);
       %Vizualizirane 
       imshow(TrackedObject);
       %Zapiswane na kadura "F" ot figurata w AVI obekta "aviobj"
       %F = getframe;
       %aviobj = addframe(aviobj,F);
       %Izchisliawane na nowite koordinati na prozoreca chrez koito shte
       %bude izriazano nowo parche na kandidat-celta, ot kadura.
       %Newcoords(1) - xmin
       %Newcoords(2) - ymin
       Newcoords=[Newcoords(1)+MhX Newcoords(2)-MhY MaxTemplateSize MaxTemplateSize];
       %%
   end
   
       
   if (flag==0)
       %%Poluchawane na model na celta
       %Purwonachalen kadur
       InitialFrame=Frame;
       %Izriazwane na proizwolno parche ot kadura obhwashtashto celta
       [InitTarget coords]=imcrop(InitialFrame);
       %Opredeliane na razmera na parchto
       [TemplateSizeY TemplateSizeX ~]=size(InitTarget);
       %Opredeliane na maksimalnia razmer na towa parche (po X ili po Y)
       MaxTemplateSize=max(TemplateSizeY,TemplateSizeX);
       %Izchisliawane na nowi koordinati na prozoreca, chrez koito shte se
       %izreje nowo parche, taka 4e to da bude kwadratno za da se izbegne
       %izchisliawane na normalizirani pikselni koordinati [0,1](w edinichna okrujnost) i se
       %izpolzwat koordinatite ot dekartowata koordinatna sistema s
       %neopredelen maksimalen razmer (w zawisimost ot razmera na
       %parcheto).Izbiagwat se uslojnenia pri izchisliawaneto i na maskata
       %na Epanechnikov i posledwashto preobrazuwane ot normalizirani
       %pikselni koordinati w standartni.
       Newcoords=[coords(1) coords(2) MaxTemplateSize MaxTemplateSize];
       %Izchisliawane na purwonachalnite koordinati na obekta w
       %koordinatnata sistema na kadura
       y0=round(coords(2)+MaxTemplateSize/2);
       x0=round(coords(1)+MaxTemplateSize/2);
       %Izriazwane na nowo (kwadratno) parche ot kadura
       Target=imcrop(InitialFrame,Newcoords);
       %Preobrazuwane w iarkostno
       GrayTarget=rgb2gray(Target);
       %Generirane na maskata na Epanechnikov wuw funkcia ot razmera na
       %parcheto. Tazi maska se izpolzwa mnogokratno i sled towa pri
       %izchisliawane na modela na kandidat-celta
       EpanechnikovMask=EpanechnikovKernel(MaxTemplateSize);
       %EpanechnikovMask=EpanechnikovMask*255;
       %EpanechnikovMask=uint8(EpanechnikovMask);
       %Preobrazuwane na tipa danni s cel suwmestimost
       GrayTarget=double(GrayTarget);
       %Izchisliawane na preteglenia model na obekta
       WeightedTarget=GrayTarget.*EpanechnikovMask;
       %Preobrazuwane w uint8 tip danni s cel zakrugliane na stoinostite i
       %poluchawane na iasna histograma. Histogramite se izchisliawat za
       %pulnia broi BIN-owe (256 na broi) sus stoinosti ot 0-255 za da se
       %izbegne posledwashto tursene na towa kum koi BIN ot histogramata
       %prinadleji stoinosta na daden piksel(wsiaka edna stoinost na pikselite suotwetstwa direktno na BIN sus sushtata stoinost)
       WeightedTarget=uint8(WeightedTarget);
       %Izchisliawane na modela na celta (histograma)
       [TargetCounts,TargetCountsX]=imhist(WeightedTarget); %Target Model
       flag=1;
      
       %Test kod
       %figure();stem(TargetCountsX,TargetCounts);
       %InitialFrame=draw_cross_marker(InitialFrame,y0,x0,255,0,0);
       %y-round(coords(2)+coords(4)/2)
       %x-round(coords(1)+coords(3)/2)
       %figure();imshow(InitialFrame);
       %figure();imshow(Target);
       %%
   end
end
%Zatwariane na AVI obekta (ako izpulnenieto na koda se prekusne ruchno (Ctrl+C) tozi red triabwa da se izpulni sushto ruchno)
%aviobj = close(aviobj);
       