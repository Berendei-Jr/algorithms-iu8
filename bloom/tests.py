import subprocess

result = subprocess.run(['cmake-build-debug/bloom', 'set 2 0.25'], stdout=subprocess.PIPE, encoding='utf-8')
print(result.stdout)
