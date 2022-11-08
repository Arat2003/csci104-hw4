#!/usr/bin/python3

import cs_grading
import cmake_problem
import setting
import os

source_dir = os.path.dirname(os.path.realpath(__file__))

RESULT_DIR = 'results/' # where test results are stored
GRADER_CONFIG = '../grader.txt'
RUBRIC_GENERAL = os.path.join(source_dir, 'rubric', 'general.config')
RUBRIC_EQUALPATHS = os.path.join(source_dir, 'rubric', 'equalpaths.config')
RUBRIC_BST = os.path.join(source_dir, 'rubric', 'bst.config')
RUBRIC_AVLBST = os.path.join(source_dir, 'rubric', 'avl.config')

GRADE_REPORT_DIR = './'

HOMEWORK = cs_grading.Homework(
    4,
    RESULT_DIR,
    False,
    detailed_results=setting.DETAILED_RESULT,
    compile_flags=setting.COMPILE_FLAGS,
    logging_level=setting.LOGGING_LEVEL,
)

P2 = cs_grading.Problem(HOMEWORK, 2, 'equalpaths', 8)
P3 = cs_grading.Problem(HOMEWORK, 3, 'bst', 30)
P4 = cs_grading.Problem(HOMEWORK, 4, 'avl', 34)

for problem, rubric in [(P2, RUBRIC_EQUALPATHS), (P3, RUBRIC_BST), (P4, RUBRIC_AVLBST)]:

    problem.generate_results(
        cmake_problem.cmake_problem,
        True,
        timeout=0,)
    if setting.GENERATE_GRADE_REPORT:
        problem.grade_problem(RUBRIC_GENERAL, rubric)
    if setting.OPEN_RESULT:
        problem.open_result(text_editor=setting.TEXT_EDITOR)


GRADER = cs_grading.Grader(GRADER_CONFIG, setting.LOGGING_LEVEL)
cs_grading.generate_grade_report(HOMEWORK, GRADER, GRADE_REPORT_DIR, overwrite=True, logging_level=setting.LOGGING_LEVEL)
