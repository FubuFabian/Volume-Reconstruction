sigma = 3.5;
hSize = sigma*3;
wSize=2.*round(((2*hSize)+1)/2)-1;
wCenter = ceil(wSize/2); 

gaussKernel= zeros(wSize,wSize,wSize);

for i=1:wSize
    for j=1:wSize
        for k=1:wSize
            
            radius = (i-wCenter)^2 + (j-wCenter)^2 + (k-wCenter)^2;
            gaussKernel(i,j,k) = exp(-radius/(2*sigma^2));
            
        end
    end
end
    

	
% 	gaussKernel.resize(wSize);
% 	for (int i = 0; i < wSize; ++i) {
% 		gaussKernel[i].resize(wSize);
% 		for (int j = 0; j < wSize; ++j)
% 			gaussKernel[i][j].resize(wSize);
%     }
% 
% 	for(int i=0;i<wSize;i++){
% 		for(int j=0;j<wSize;j++){
% 			for(int k=0;k<wSize;k++){
% 
% 				float radius = pow(double(i-wCenter),2) + pow(double(j-wCenter),2) + pow(double(k-wCenter),2);
% 				gaussKernel[i][j][k] = exp(-radius/(2*pow(sigma,2)));
% 
% 
% 			}
% 		}
% 	}