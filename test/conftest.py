
def pytest_addoption(parser):
    parser.addoption("--set_label", action="store", default="1", help="First argument")
    parser.addoption("--boundary_type", action="store", default="toroid", help="Second argument")
    parser.addoption("--dispersal_type", action="store", default="distance_kernel", help="Third argument")