from selenium.webdriver.firefox.options import Options
from selenium import webdriver

options = Options()
options.headless = True
driver = webdriver.Firefox(options=options)
driver.get("https://sklep.pgg.pl")
pageSource = driver.page_source
fileToWrite = open("page.html", "w")
fileToWrite.write(pageSource)
fileToWrite.close()
driver.quit()
