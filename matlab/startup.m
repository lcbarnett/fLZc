% Startup stuff

fprintf('[fLZc startup] Initialising fLZc\n');

% Add LZc root dir + appropriate subdirs to path

global fLZc_matlab
fLZc_matlab = fileparts(mfilename('fullpath')); % directory containing this file
addpath(genpath(fLZc_matlab)); % add subdirectories

global fLZc_root
fLZc_root = fileparts(fLZc_matlab);
fprintf('[fLZc startup] Added path %s\n',fLZc_matlab);

global fLZc_data_path
fLZc_data_path = fullfile(fLZc_matlab,'data');
fprintf('[fLZc startup] Normalisation data path is %s\n',fLZc_data_path);

% Binary LZc normalisation data: download zip file (~ 100 MB)
%
%    http://users.sussex.ac.uk/~lionelb/downloads/fLZc_matlab_data.zip
%
% and unzip into fLZc_data_path

% Check if normalisation data available

check_norm_data('76');
check_norm_data('78');

fprintf('[fLZc startup] Initialised (you may re-run `startup'' at any time)\n');

function check_norm_data(ver)

	global fLZc_data_path
	dl = dir([fLZc_data_path filesep 'LZ' ver 'c_rand_a*.mat']);
	abets = cellfun(@str2num,extract({dl.name},digitsPattern));
	nabs = length(abets);
	if nabs == 0
		fprintf('[fLZc startup]\n');
		fprintf(2,'[fLZc startup] WARNING: No LZ%sc normalisation data files found.\n',ver);
		fprintf(2,'[fLZc startup]          Download from http://users.sussex.ac.uk/~lionelb/downloads/fLZc_matlab_data.zip\n');
		fprintf(2,'[fLZc startup]          and unzip into the directory ''%s''.\n',fLZc_data_path);
		fprintf(2,'[fLZc startup]          Accurate normalisation will be unavailable (see ''LZc_crand.m'')\n');
		fprintf('[fLZc startup]\n');
	else
		fprintf('[fLZc startup] Normalisation data found for alphabet sizes %d',abets(1))
		for i = 2:nabs
			fprintf(', %d',abets(i));
		end
		fprintf('\n');
	end

end
