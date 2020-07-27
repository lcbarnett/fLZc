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

fprintf('[LZc startup] Initialised (you may re-run `startup'' at any time)\n');
