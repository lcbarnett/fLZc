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
% then extract and set the environmental variable 'FLZC_DATA_PATH' to the
% resulting directory 'fLZc_data'

global fLZc_data_path;
fLZc_data_path = getenv('FLZC_DATA_PATH');
if isempty(fLZc_data_path)
	fprintf(2,'[fLZc startup] WARNING: LZc normalisation data not found; normalisation will not be available\n');
else
	assert(exist(fLZc_data_path,'dir') == 7,'bad data path: ''%s'' does not exist or is not a directory',fLZc_data_path);
end

fprintf('[fLZc startup] Initialised (you may re-run `startup'' at any time)\n');
