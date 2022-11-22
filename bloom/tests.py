from subprocess import Popen, PIPE
import re

errors = 0
for i in range(22):
    input = ''
    answers = []
    stream = open(f'input/{i+1}.txt', 'r')
    answersStream = open(f'output/{i+1}.txt', 'r')

    for line in stream:
        input += line
    for line in answersStream:
        answers.append(line)    

    stdout_data = Popen(['_build/bloom'], stdout=PIPE, stdin=PIPE, stderr=PIPE).communicate(input.encode())[0].decode()

    outputStream = open(f'myOutput/{i+1}.txt', 'w')
    output = []
    curStr = ''
    for char in stdout_data:
        if char == '\n':
            output.append(curStr)
            outputStream.write(curStr+'\n')
            curStr = ''
            continue
        curStr += char

    #print(f'answers: {len(answers)}')
    #print(f'output: {len(output)}')
    if len(answers) != len(output):
        print(f'error in test {i+1}: разное количество строк')
        continue
    n = len(answers)
    if n == 0:
        continue
    for i in range(n):
        if answers[i][-1] == '\n':
            answers[i] = re.sub('[\t\r\n]', '', answers[i])
        if output[i] != answers[i]:
            errors += 1
            print('\nError:\nYour output:')
            print(output[i])
            print('Correct output:')
            print(answers[i])          

print(f'Overall errors: {errors}')    
