function [MhY MhX]=MeanShiftVectEstimate(WeightedCandidateTarget,TargetCounts,TargetCountsX,CandidateTargetCounts,CandidateTargetCountsX)
%Opredeliane razmera na parcheto na kandidat-celta
[CandImSizeY CandImSizeX]=size(WeightedCandidateTarget);
%Inicializirane na masiwa w koito shte budat zapiswani stoinostite na
%koeficientite wi. wi se izchisliawa za wseki piksel pootdelno.
Wi=zeros(CandImSizeY,CandImSizeX);
%Inicializirane na stoinostite na sumite ot proizwedenieta Wi*Xi (Xi e mestopolojenieto na piksela)
SumWiY=0;
SumWiX=0;
%Namirane na centura na parcheto (koordinati w pikseli)
CenterY=round(CandImSizeY/2);
CenterX=round(CandImSizeX/2);

for i=1:1:CandImSizeX
    for j=1:1:CandImSizeY
        %Inicializirane na tekushto izchisliawanata stoinost za piksel xi
        wi=0;
        %Izchisliawane na delta funkciata na Kroneker za daden
        %piksel
        PixValue=double(WeightedCandidateTarget(j,i));
        for b=1:1:256
           Difference=PixValue-(b-1);
           if(Difference) DeltaFunction=0;
           else DeltaFunction=1;
           wi=wi+DeltaFunction*sqrt(TargetCounts(b)/CandidateTargetCounts(b));
           end
        end
        %Zapiswane na stoinostite w masiw
        Wi(j,i)=wi;
    end
end
%Izchisliawane na sumite po formula
for i=1:1:CandImSizeX
    for j=1:1:CandImSizeY
       SumWiY=SumWiY+Wi(j,i)*(CenterY-j);
       SumWiX=SumWiX+Wi(j,i)*(i-CenterX);
    end
end
WiSum=sum(sum(Wi));
%Izchisliawane na X i Y parametrite na Mean-Shift vektora
MhY=SumWiY/WiSum;
MhX=SumWiX/WiSum;


        
    
    