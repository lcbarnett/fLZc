% Startup stuff

fprintf('[fLZc startup] Initialising fLZc\n');

% Add LZc root dir + appropriate subdirs to path

global fLZc_root;
fLZc_root = fileparts(mfilename('fullpath')); % directory containing this file
addpath(genpath(fLZc_root));
fprintf('[fLZc startup] Added path %s\n',fLZc_root);

% Binary data files used in *_crand.m: download zip file
%
% http://users.sussex.ac.uk/~lionelb/downloads/fLZc_data.zip
%
% then extract .mat files into <fLZc_root>/data/

global fLZc_data_path;
fLZc_data_path = fullfile(fLZc_root,'data');

% Check if data seems to be there

df1 = length({dir([fLZc_data_path filesep '/LZc_rand_A*.mat']).name});
df2 = length({dir([fLZc_data_path filesep '/LZ76c_rand_A*.mat']).name});
if ~(df1 == 20 && df2 == 20)
	fprintf('[fLZc startup]\n');
	fprintf(2,'[fLZc startup] WARNING: Some Matlab LZc normalisation data files seem to be missing.\n');
	fprintf(2,'[fLZc startup]          Download from http://users.sussex.ac.uk/~lionelb/downloads/fLZc_data.zip\n');
	fprintf(2,'[fLZc startup]          and unzip into the directory ''%s''.\n',fLZc_data_path);
	fprintf(2,'[fLZc startup]          Normalisation will be unavailable (see ''LZc_crand.m'' and ''LZ76c_crand.m'')\n');
	fprintf('[fLZc startup]\n');
else
	fprintf('[fLZc startup] LZc normalisation data appears to be present and correct\n');
end

fprintf('[fLZc startup] Initialised (you may re-run `startup'' at any time)\n');
