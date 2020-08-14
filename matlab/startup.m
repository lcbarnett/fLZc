% Startup stuff

global LZc_version;
LZc_version.major = 1;
LZc_version.minor = 0;

fprintf('[LZc startup] Initialising LZc version %d.%d\n', LZc_version.major, LZc_version.minor);

% Add LZc root dir + appropriate subdirs to path

global LZc_root;
LZc_root = fileparts(mfilename('fullpath')); % directory containing this file
addpath(genpath(LZc_root));
fprintf('[LZc startup] Added path %s\n',LZc_root);

% Binary data files: download zip file
%
% http://users.sussex.ac.uk/~lionelb/downloads/fLZc_data.zip
%
% then extract and set the environmental variable 'FLZC_DATA_PATH' to the
% resulting directory 'fLZc_data'

global fLZc_data_path;
fLZc_data_path = getenv('FLZC_DATA_PATH');
assert(~isempty(fLZc_data_path), 'You must define the environmental variable ''FLZC_DATA_PATH''');
assert(exist(fLZc_data_path,'dir') == 7,'bad data path: ''%s'' does not exist or is not a directory',fLZc_data_path);

fprintf('[LZc startup] Initialised (you may re-run `startup'' at any time)\n');
