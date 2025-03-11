function LZc_gen_rand(n,d,N,s)

% Estimate means and (optionally) standard deviations of LZc for random strings
% with alphabet size d of length n, based on a sample of size N. If the flag
% 'allvals' evaluates as true, then means and standard deviations are calculated
% for all lengths up to and including n.

if nargin < 4, s = []; end % LZc_rand default

fprintf('\nRunning LZc_gen_rand (n = %d, d = %d, N = %d) ...',n,d,N);
st = tic;
[cmean,csdev] = LZc_rand(n,d,N,true,s);
et = toc(st);
fprintf(' done : computation time = %s\n\n',datestr(seconds(et),'HH:MM:SS.FFF'));

global fLZc_data_path;
fname = fullfile(fLZc_data_path,sprintf('LZc_rand_a%02d.mat',d));
fprintf('Saving data file ''%s'' ...',fname);
save(fname,'n','d','N','cmean','csdev');
l = dir(fname);
fprintf(' done : %.2f Mb\n\n',l.bytes/(1000*1000));
