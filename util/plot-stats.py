import matplotlib.pyplot as plt
import matplotlib.figure as fig
import numpy as np

# open the data
dist_data = np.genfromtxt('../model/mqms_roundrobin.csv', delimiter=',')
print(dist_data.shape)

# set up the plot's axes
w, h = fig.figaspect(4/3)
fig, axs = plt.subplots(3, 1, sharex=True, sharey=False)
titles = [ # the titles for the subplots
  'Utilization',
  'Service Time (s)',
  'Number of Jobs'
]
ylabels = [
  'utilization (%)',
  'time (s)',
  '# jobs'
]

# plot in the subplots
for i, ax in enumerate(axs.flat):
  # calculate the value frequencies
  # uniq, freq = np.unique(dist_data[i, :], return_counts=True)
  # freq = freq / np.sum(freq)

  # plot the histogram
  ax.scatter(dist_data[1:,0], dist_data[1:,i+1], c='k', marker='o', s=1)
  ax.set_title(titles[i])
  ax.set_ylabel(ylabels[i])
  maxVal = np.max(dist_data[1:,i+1])
  maxVal += maxVal * 0.05
  ax.set_ylim([0, maxVal])
  # plt.setp(markerline, markersize = 3)
  # plt.setp(stemline, linewidth = 1.25)

# set up the shared axes labels etc.
fig.add_subplot(111, frameon=False)
plt.tick_params(labelcolor='none', top=False, bottom=False, left=False, \
  right=False)
plt.xlabel('Service Node ID')
# fig.subplots_adjust(bottom=-0.1)

# save the figure
plt.savefig("test-dist.png", dpi=300)
