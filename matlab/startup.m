% Startup stuff

fprintf('[fLZc startup] Initialising fLZc\n');

% Add LZc root dir + appropriate subdirs to path

global fLZc_matlab fLZc_root fLZc_data_path
fLZc_matlab = fileparts(mfilename('fullpath')); % directory containing this file
fLZc_root = fileparts(fLZc_matlab);
addpath(genpath(fLZc_matlab));
fprintf('[fLZc startup] Added path %s\n',fLZc_matlab);
fLZc_data_path = fullfile(fLZc_matlab,'data');
fprintf('[fLZc startup] Normalisation data path is %s\n',fLZc_data_path);

% Binary data files used in *_crand.m: download zip file
%
%    http://users.sussex.ac.uk/~lionelb/downloads/fLZc_data.zip
%
% then extract .mat files into fLZc_data_path

% Check if normalisation data available

dl = dir([fLZc_data_path filesep 'LZc_rand_a*.mat']);
abets = cellfun(@str2num,extract({dl.name},digitsPattern));
nabs = length(abets);
if nabs == 0
	fprintf('[fLZc startup]\n');
	fprintf(2,'[fLZc startup] WARNING: No LZc normalisation data files found.\n');
	fprintf(2,'[fLZc startup]          Download from http://users.sussex.ac.uk/~lionelb/downloads/fLZc_data.zip\n');
	fprintf(2,'[fLZc startup]          and unzip into the directory ''%s''.\n',fLZc_data_path);
	fprintf(2,'[fLZc startup]          Normalisation will be unavailable (see ''LZc_crand.m'')\n');
	fprintf('[fLZc startup]\n');
else
	fprintf('[fLZc startup] Normalisation data found for alphabet sizes %d',abets(1))
	for i = 2:nabs
		fprintf(', %d',abets(i));
	end
	fprintf('\n');
end
clear dl abets nabs i

fprintf('[fLZc startup] Initialised (you may re-run `startup'' at any time)\n');
