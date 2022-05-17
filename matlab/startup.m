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
fLZc_data_path = fullfile(fLZc_root,'data');

fprintf('[fLZc startup] Initialised (you may re-run `startup'' at any time)\n');
