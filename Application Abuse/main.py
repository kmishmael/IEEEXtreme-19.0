import sys
from collections import defaultdict
from datetime import datetime
import re

def parse_rules(rules_line):
    rules = {}
    parts = rules_line.strip().split(',')
    for part in parts:
        part = part.strip()
        key, value = part.split('=')
        rules[key.strip()] = int(value.strip())
    return rules

def parse_descriptor(descriptor_line):
    fields = [f.strip() for f in descriptor_line.strip().split(',')]
    field_map = {}
    for i, field in enumerate(fields):
        field_map[field] = i
    return field_map

def extract_date(date_str):
    date_str = date_str.strip('[]')
    dt = datetime.strptime(date_str, '%d/%b/%Y:%H:%M:%S')
    return dt.strftime('%d/%b/%Y'), dt

def parse_log_entry(line, field_map):
    parts = []
    current = []
    in_quotes = False
    in_brackets = False
    
    i = 0
    while i < len(line):
        char = line[i]
        
        if char == '[' and not in_quotes:
            in_brackets = True
            current.append(char)
        elif char == ']' and not in_quotes and in_brackets:
            in_brackets = False
            current.append(char)
        elif char == '"':
            in_quotes = not in_quotes
            current.append(char)
        elif char == ' ' and not in_quotes and not in_brackets:
            if current:
                parts.append(''.join(current))
                current = []
        else:
            current.append(char)
        
        i += 1
    
    if current:
        parts.append(''.join(current))
    
    entry = {}
    for field_name, idx in field_map.items():
        if idx < len(parts):
            value = parts[idx]
            if value.startswith('"') and value.endswith('"'):
                value = value[1:-1]
            if value.startswith('[') and value.endswith(']'):
                value = value[1:-1]
            entry[field_name] = value if value != '-' else None
        else:
            entry[field_name] = None
    
    return entry

def extract_pdf_number(request):
    match = re.search(r'/document/(\d+)\.pdf', request)
    if match:
        return int(match.group(1))
    return None

def check_sequential_crawl(pdf_downloads, crawl_limit):
    if len(pdf_downloads) < crawl_limit:
        return 0
    
    max_sequential = 0
    i = 0
    
    while i < len(pdf_downloads):
        current_seq = 1
        j = i + 1
        expected_next = pdf_downloads[i] + 1
        
        while j < len(pdf_downloads):
            if pdf_downloads[j] == expected_next:
                current_seq += 1
                expected_next += 1
                j += 1
            else:
                break
        
        max_sequential = max(max_sequential, current_seq)
        i += 1
    
    return max_sequential if max_sequential >= crawl_limit else 0

def main():
    lines = []
    for line in sys.stdin:
        lines.append(line.rstrip('\n'))
    
    rules = parse_rules(lines[0])
    
    field_map = parse_descriptor(lines[1])
    
    user_data = defaultdict(lambda: defaultdict(lambda: {
        'agents': set(),
        'ips': set(),
        'sessions': set(),
        'pdfs': [],
        'pdf_numbers': []
    }))
    
    for i in range(2, len(lines)):
        entry = parse_log_entry(lines[i], field_map)
        
        if not entry.get('Id') or entry.get('HTTP Status') != '200':
            continue
        
        user_id = entry['Id']
        date_str, _ = extract_date(entry['Date'])
        
        if entry.get('User Agent'):
            user_data[user_id][date_str]['agents'].add(entry['User Agent'])
        
        if entry.get('Client IP'):
            user_data[user_id][date_str]['ips'].add(entry['Client IP'])
        
        if entry.get('Session Cookie'):
            user_data[user_id][date_str]['sessions'].add(entry['Session Cookie'])
        
        pdf_num = extract_pdf_number(entry.get('Request', ''))
        if pdf_num is not None:
            user_data[user_id][date_str]['pdfs'].append(entry['Request'])
            user_data[user_id][date_str]['pdf_numbers'].append(pdf_num)
    
    violations = []
    
    for user_id in user_data:
        for date in user_data[user_id]:
            data = user_data[user_id][date]
            
            if 'agent' in rules and len(data['agents']) >= rules['agent']:
                violations.append((user_id, 'agent', len(data['agents'])))
            
            if 'ip' in rules and len(data['ips']) >= rules['ip']:
                violations.append((user_id, 'ip', len(data['ips'])))
            
            if 'pdf' in rules and len(data['pdfs']) >= rules['pdf']:
                violations.append((user_id, 'pdf', len(data['pdfs'])))
            
            if 'session' in rules and len(data['sessions']) >= rules['session']:
                violations.append((user_id, 'session', len(data['sessions'])))
            
            if 'crawl' in rules:
                seq_count = check_sequential_crawl(data['pdf_numbers'], rules['crawl'])
                if seq_count > 0:
                    violations.append((user_id, 'crawl', seq_count))
    
    violations.sort(key=lambda x: (x[0], x[1]))
    
    if not violations:
        print("N/A")
    else:
        for user_id, abuse_type, count in violations:
            print(f"{user_id} {abuse_type}={count}")

if __name__ == "__main__":
    main()