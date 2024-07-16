import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages

# Read data from CSV file
data = pd.read_csv('data.csv')

# Plotting function
def plot_page_faults(data, page_range, page_string_length, pdf):
    subset = data[(data['Page Range'] == page_range) & (data['Page String Length'] == page_string_length)]
    
    plt.figure(figsize=(10, 6))
    plt.plot(subset['Frames'], subset['FIFO Page Faults'], marker='o', label='FIFO Page Faults')
    plt.plot(subset['Frames'], subset['LRU Page Faults'], marker='o', label='LRU Page Faults')
    plt.plot(subset['Frames'], subset['Optimal Page Faults'], marker='o', label='Optimal Page Faults')
    
    plt.title(f'Page Faults vs Frames (Page Range: {page_range}, Page String Length: {page_string_length})')
    plt.xlabel('Frames')
    plt.ylabel('Page Faults')
    plt.legend()
    plt.grid(True)
    plt.xticks(subset['Frames'])
    
    pdf.savefig()  # Save the current figure into the PDF
    plt.close()  # Close the figure to free memory

# Create a PDF file to store the plots
with PdfPages('plot.pdf') as pdf:
    page_ranges = data['Page Range'].unique()
    page_string_lengths = data['Page String Length'].unique()

    for page_range in page_ranges:
        for page_string_length in page_string_lengths:
            plot_page_faults(data, page_range, page_string_length, pdf)

print("All plots have been saved to 'plot.pdf'")
