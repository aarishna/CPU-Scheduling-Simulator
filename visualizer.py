import json
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import sys
import numpy as np

def load_json(filename):
    """Load scheduling results from JSON file"""
    with open(filename, 'r') as f:
        return json.load(f)

def generate_gantt_chart(data, output_file=None):
    """Generate Gantt chart from scheduling data"""
    algorithm = data['algorithm']
    
    # Support both timeline and gantt_chart formats
    gantt_data = data.get('timeline', data.get('gantt_chart', []))
    
    if not gantt_data:
        print("Error: No timeline or gantt_chart data found!")
        return
    
    metrics = data['metrics']
    
    # Create figure
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(16, 10), 
                                     gridspec_kw={'height_ratios': [3, 1]})
    
    # Color map for processes
    unique_pids = sorted(set(entry['pid'] for entry in gantt_data))
    colors = plt.cm.Set3(np.linspace(0, 1, len(unique_pids)))
    color_map = {pid: colors[i] for i, pid in enumerate(unique_pids)}
    
    # Plot Gantt chart
    for entry in gantt_data:
        pid = entry['pid']
        start = entry['start']
        end = entry['end']
        duration = end - start
        
        ax1.barh(f'P{pid}', duration, left=start, height=0.6,
                color=color_map[pid], edgecolor='black', linewidth=1.5)
        
        # Add time labels on bars
        if duration > 1:
            ax1.text(start + duration/2, f'P{pid}', f'{duration}',
                    ha='center', va='center', fontsize=9, fontweight='bold')
    
    # Formatting for Gantt chart
    ax1.set_xlabel('Time', fontsize=12, fontweight='bold')
    ax1.set_ylabel('Process ID', fontsize=12, fontweight='bold')
    ax1.set_title(f'{algorithm.upper()} Scheduling - Gantt Chart', 
                 fontsize=16, fontweight='bold', pad=20)
    ax1.grid(True, axis='x', alpha=0.3, linestyle='--')
    ax1.set_xlim(0, max(entry['end'] for entry in gantt_data))
    
    # Add vertical lines at each time unit
    max_time = max(entry['end'] for entry in gantt_data)
    for t in range(0, max_time + 1, 5):
        ax1.axvline(x=t, color='gray', linestyle=':', alpha=0.5, linewidth=0.8)
    
    # Plot metrics as a table
    ax2.axis('tight')
    ax2.axis('off')
    
    metric_data = [
        ['Metric', 'Value'],
        ['Average Waiting Time', f"{metrics['avg_waiting_time']:.2f}"],
        ['Average Turnaround Time', f"{metrics['avg_turnaround_time']:.2f}"],
        ['Average Response Time', f"{metrics['avg_response_time']:.2f}"],
        ['CPU Utilization', f"{metrics['cpu_utilization']:.2f}%"],
        ['Throughput', f"{metrics['throughput']:.4f} processes/unit"]
    ]
    
    table = ax2.table(cellText=metric_data, cellLoc='left',
                     colWidths=[0.4, 0.3], loc='center',
                     bbox=[0.1, 0.0, 0.8, 1.0])
    
    table.auto_set_font_size(False)
    table.set_fontsize(11)
    table.scale(1, 2)
    
    # Style the header row
    for i in range(2):
        table[(0, i)].set_facecolor('#4CAF50')
        table[(0, i)].set_text_props(weight='bold', color='white')
    
    # Alternate row colors
    for i in range(1, len(metric_data)):
        for j in range(2):
            if i % 2 == 0:
                table[(i, j)].set_facecolor('#f0f0f0')
    
    plt.tight_layout()
    
    if output_file:
        plt.savefig(output_file, dpi=300, bbox_inches='tight')
        print(f" Gantt chart saved to {output_file}")
    
    plt.show()

def generate_comparison_chart(json_files):
    """Generate comparison chart for multiple algorithms"""
    fig, axes = plt.subplots(2, 3, figsize=(18, 10))
    fig.suptitle('CPU Scheduling Algorithm Comparison', 
                 fontsize=18, fontweight='bold')
    
    algorithms = []
    metrics_data = {
        'avg_waiting_time': [],
        'avg_turnaround_time': [],
        'avg_response_time': [],
        'cpu_utilization': [],
        'throughput': []
    }
    
    # Load all data
    for filename in json_files:
        try:
            data = load_json(filename)
            algorithms.append(data['algorithm'].upper())
            for key in metrics_data.keys():
                metrics_data[key].append(data['metrics'][key])
        except Exception as e:
            print(f"Error loading {filename}: {e}")
            continue
    
    if len(algorithms) < 2:
        print("Not enough valid data files for comparison")
        return
    
    # Plot each metric
    metrics_info = [
        ('avg_waiting_time', 'Average Waiting Time', 'time units'),
        ('avg_turnaround_time', 'Average Turnaround Time', 'time units'),
        ('avg_response_time', 'Average Response Time', 'time units'),
        ('cpu_utilization', 'CPU Utilization', '%'),
        ('throughput', 'Throughput', 'processes/unit')
    ]
    
    colors = ['#FF6B6B', '#4ECDC4', '#45B7D1', '#FFA07A', '#98D8C8']
    
    for idx, (metric_key, title, unit) in enumerate(metrics_info):
        row = idx // 3
        col = idx % 3
        ax = axes[row, col]
        
        bars = ax.bar(algorithms, metrics_data[metric_key], 
                     color=colors[:len(algorithms)])
        ax.set_title(title, fontweight='bold', fontsize=12)
        ax.set_ylabel(unit, fontsize=10)
        ax.grid(True, axis='y', alpha=0.3)
        
        # Add value labels on bars
        for bar in bars:
            height = bar.get_height()
            ax.text(bar.get_x() + bar.get_width()/2., height,
                   f'{height:.2f}',
                   ha='center', va='bottom', fontsize=9, fontweight='bold')
        
        # Rotate x-axis labels
        ax.tick_params(axis='x', rotation=45)
    
    # Hide the last subplot (2,2)
    axes[1, 2].axis('off')
    
    plt.tight_layout()
    plt.savefig('algorithm_comparison.png', dpi=300, bbox_inches='tight')
    print(" Comparison chart saved to algorithm_comparison.png")
    plt.show()

def main():
    if len(sys.argv) < 2:
        print("Usage:")
        print("  python visualizer.py <json_file>           - Single algorithm")
        print("  python visualizer.py -c <file1> <file2>... - Compare algorithms")
        return
    
    if sys.argv[1] == '-c':
        # Comparison mode
        json_files = sys.argv[2:]
        if len(json_files) < 2:
            print("Error: Need at least 2 JSON files for comparison")
            return
        generate_comparison_chart(json_files)
    else:
        # Single algorithm mode
        json_file = sys.argv[1]
        try:
            data = load_json(json_file)
            output_image = json_file.replace('.json', '.png')
            generate_gantt_chart(data, output_image)
        except FileNotFoundError:
            print(f"Error: File '{json_file}' not found!")
        except Exception as e:
            print(f"Error: {e}")

if __name__ == '__main__':
    main()