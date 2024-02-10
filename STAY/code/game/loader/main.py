import re_stay as rs

class Loader(rs.ILoader):
    def load(self, path: str, node: rs.Node):
        print('Nothing to load just yet')
        print(f'However the entity is {node.entity()}')

loader = Loader()
rs.setLoader(loader)

rs.run()