class AISpawnerEntityClass: DAD_SpawnRadiusEntityClass
{
	
}

class AISpawnerEntity: DAD_SpawnRadiusEntity 
{
	protected ref array<DAD_GroupSpawnPoint> m_SpawnPoints;
	protected static int m_iGroupID = 0;
	
	private void CreateSpawns()
	{
	
	}

	override void OnActivate(IEntity ent)
	{
		Resource us   = Resource.Load("{63F01153A435D2BD}Prefabs/SpawnUS.et");
		Resource ussr = Resource.Load("{63F01153A435D2BD}Prefabs/SpawnUSSR.et");
		
		super.OnActivate(ent);
		
		foreach (int i, IEntity e : m_Entities)
		{
			auto group = SCR_AIGroup.Cast(e);
			group.SetName("Squad " + m_iGroupID++);

			string fac = group.m_faction;
			Resource res;
			if (fac == "US")
				res = us;
			else
				res = ussr;

			DAD_GroupSpawnPoint spawn = DAD_GroupSpawnPoint.Cast(SpawnHelpers.SpawnEntity(res, group.GetOrigin()));

			spawn.SetFactionKey(fac);
			spawn.SetGroup(group);
		}
	}

};
